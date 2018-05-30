/*******************************************************************************
 * Copyright (C) Gallium Studio LLC. All rights reserved.
 *
 * This program is open source software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * Alternatively, this program may be distributed and modified under the
 * terms of Gallium Studio LLC commercial licenses, which expressly supersede
 * the GNU General Public License and are specifically designed for licensees
 * interested in retaining the proprietary status of their code.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * Contact information:
 * Website - https://www.galliumstudio.com
 * Source repository - https://github.com/galliumstudio
 * Email - admin@galliumstudio.com
 ******************************************************************************/

#ifndef FW_HSM_H
#define FW_HSM_H

#include <stdint.h>
#include "qpcpp.h"
#include "fw_def.h"
#include "fw_defer.h"
#include "fw_map.h"
#include "fw_evt.h"
#include "fw_assert.h"

#define FW_HSM_ASSERT(t_) ((t_) ? (void)0 : Q_onAssert("fw_hsm.h", (int_t)__LINE__))


#define GET_ID()    me->GetHsm().GetId()
#define GEN_SEQ()   me->GetHsm().GenSeq()

namespace FW {

class Active;
class Region;

typedef KeyValue<HsmId, Sequence> HsmSeq;
typedef Map<HsmId, Sequence> HsmSeqMap;

class Hsm {
public:
    Hsm(HsmId id, char const *name, Active *active, Region *region,
        EvtName timerEvtName, EvtCount timerEvtCount,
        EvtName internalEvtName, EvtCount internalEvtCount,
        EvtName interfaceEvtName, EvtCount interfaceEvtCount) :
        m_id(id), m_name(name), m_active(active), m_region(region), m_state(m_undefName),
        m_timerEvtName(timerEvtName), m_timerEvtCount(timerEvtCount),
        m_internalEvtName(internalEvtName), m_internalEvtCount(internalEvtCount),
        m_interfaceEvtName(interfaceEvtName), m_interfaceEvtCount(interfaceEvtCount),
        m_nextSequence(0), m_inHsmSeq(HSM_UNDEF, 0),
        m_outHsmSeqMap(m_outHsmSeqStor, ARRAY_COUNT(m_outHsmSeqStor), HsmSeq(HSM_UNDEF, 0)) {}
    void Init(Active *container);

    HsmId GetId() const { return m_id; }
    char const *GetName() const { return m_name; }
    Active *GetActive() { return m_active; }
    Region *GetRegion() { return m_region; }
    char const *GetState() const { return m_state; }
    void SetState(char const *s) { m_state = s; }
    static char const *GetBuiltinEvtName(QP::QSignal signal);
    char const *GetEvtName(QP::QSignal signal) const;
    static char const *GetUndefName() { return m_undefName; }

    Sequence GenSeq() { return m_nextSequence++; }
    bool Defer(QP::QEvt const *e) { return m_deferEQueue.Defer(e); }
    void Recall() { m_deferEQueue.Recall(); }

    void ResetOutSeq() { m_outHsmSeqMap.Reset(); }
    void SaveOutSeq(HsmId hsm, Sequence seq) {
        FW_HSM_ASSERT(hsm != HSM_UNDEF);
        m_outHsmSeqMap.Save(HsmSeq(hsm, seq));
    }
    void SaveOutSeq(Evt const &e) { SaveOutSeq(e.GetTo(), e.GetSeq()); }
    bool MatchOutSeq(HsmId hsm, Sequence seqToMatch, bool autoClear = true);
    bool MatchOutSeq(Evt const &e, bool autoClear = true) {
        return MatchOutSeq(e.GetFrom(), e.GetSeq(), autoClear);
    }
    void ClearOutSeq(HsmId hsm) { m_outHsmSeqMap.ClearByKey(hsm); }
    bool IsOutSeqCleared(HsmId hsm) {
        FW_HSM_ASSERT(hsm != HSM_UNDEF);
        return (m_outHsmSeqMap.GetByKey(hsm) == NULL);
    }
    bool IsOutSeqAllCleared() { return (m_outHsmSeqMap.GetUsedCount() == 0); }

    void SaveInSeq(HsmId hsm, Sequence seq) {
        m_inHsmSeq.SetKey(hsm);
        m_inHsmSeq.SetValue(seq);
    }
    void SaveInSeq(Evt const &e) { SaveInSeq(e.GetFrom(), e.GetSeq()); }
    void ClearInSeq() {
        m_inHsmSeq.SetKey(HSM_UNDEF);
        m_inHsmSeq.SetValue(0);
    }
    // Must allow HSM_UNDEF.
    HsmId GetInHsm() { return m_inHsmSeq.GetKey(); }
    Sequence GetInSeq() { return m_inHsmSeq.GetValue(); }

protected:
    enum {
        OUT_HSM_SEQ_MAP_COUNT = 8,
        DEFER_QUEUE_COUNT = 16
    };

    HsmId m_id;
    char const * m_name;
    Active *m_active;
    Region *m_region;
    char const *m_state;
    EvtName m_timerEvtName;
    EvtCount m_timerEvtCount;
    EvtName m_internalEvtName;
    EvtCount m_internalEvtCount;
    EvtName m_interfaceEvtName;
    EvtCount m_interfaceEvtCount;
    Sequence m_nextSequence;
    DeferEQueue m_deferEQueue;
    HsmSeq m_inHsmSeq;
    HsmSeq m_outHsmSeqStor[OUT_HSM_SEQ_MAP_COUNT];
    HsmSeqMap m_outHsmSeqMap;
    QP::QEvt const *m_deferQueueStor[DEFER_QUEUE_COUNT];

    static char const * const m_builtinEvtName[];
    static char const m_undefName[];
};

} // namespace FW


#endif // FW_HSM_H
