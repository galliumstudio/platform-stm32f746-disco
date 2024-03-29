/*******************************************************************************
 * Copyright (C) 2018 Gallium Studio LLC (Lawrence Lo). All rights reserved.
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

#ifndef FW_ACTIVE_H
#define FW_ACTIVE_H

#include <stdint.h>
#include "qpcpp.h"
#include "fw_hsm.h"
#include "fw_evt.h"

namespace FW {

typedef KeyValue<HsmId, Region *> HsmReg;
typedef Map<HsmId, Region *> HsmRegMap;

class Region;
class Timer;

class Active : public QP::QActive {
public:
    Active(QP::QStateHandler const initial, HsmId id, char const *name,
            EvtName timerEvtName, EvtCount timerEvtCount,
            EvtName internalEvtName, EvtCount internalEvtCount,
            EvtName interfaceEvtName, EvtCount interfaceEvtCount) :
        QP::QActive(initial),
        m_hsm(id, name, this, NULL,
              timerEvtName, timerEvtCount,
              internalEvtName, internalEvtCount,
              interfaceEvtName, interfaceEvtCount),
        m_hsmRegMap(m_hsmRegStor, ARRAY_COUNT(m_hsmRegStor), HsmReg(HSM_UNDEF, NULL)){
    }
    void Start(uint8_t prio);
    void Add(Region *reg);
    Hsm &GetHsm() { return m_hsm; }

    virtual void dispatch(QP::QEvt const * const e);

protected:
    void PostSync(Evt const *e);
    QP::QEvt const **GetEvtQueueStor(uint16_t *count);

    enum {
        MAX_REGION_COUNT = 8,
        EVT_QUEUE_COUNT = 16
    };
    Hsm m_hsm;
    HsmReg m_hsmRegStor[MAX_REGION_COUNT];
    HsmRegMap m_hsmRegMap;
    QP::QEvt const *m_evtQueueStor[EVT_QUEUE_COUNT];

    friend class Timer;
};

} // namespace FW


#endif // FW_ACTIVE_H
