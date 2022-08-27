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

#include "fw_hsm.h"
#include "fw_assert.h"

FW_DEFINE_THIS_FILE("fw_hsm.cpp")

using namespace QP;

namespace FW {

char const * const Hsm::m_builtinEvtName[] = {
    "NULL",
    "ENTRY",
    "EXIT",
    "INIT"
};

char const Hsm::m_undefName[] = "UNDEF";

void Hsm::Init(Active *container) {
    m_deferEQueue.Init(container, m_deferQueueStor, ARRAY_COUNT(m_deferQueueStor));
}

char const *Hsm::GetBuiltinEvtName(QP::QSignal signal) {
    if (signal < Q_USER_SIG) {
        return m_builtinEvtName[signal];
    }
    return m_undefName;
}

char const *Hsm::GetEvtName(QSignal signal) const {
    uint32_t index;
    if (signal < Q_USER_SIG) {
        return GetBuiltinEvtName(signal);
    } else if (IS_TIMER_EVT(signal)) {
        index = GET_TIMER_EVT_INDEX(signal);
        if (m_timerEvtName && (index < m_timerEvtCount)) {
            return m_timerEvtName[index];
        }
    } else if (IS_INTERNAL_EVT(signal)) {
        index = GET_INTERNAL_EVT_INDEX(signal);
        if (m_internalEvtName && (index < m_internalEvtCount)) {
            return m_internalEvtName[index];
        }
    } else {
        index = GET_INTERFACE_EVT_INDEX(signal);
        if (m_interfaceEvtName && (index < m_interfaceEvtCount)) {
            return m_interfaceEvtName[index];
        }
    }
    return m_undefName;
}

bool Hsm::MatchOutSeq(HsmId hsm, Sequence seqToMatch, bool autoClear) {
    if (hsm == HSM_UNDEF) {
        return false;
    }
    HsmSeq *hsmSeq = m_outHsmSeqMap.GetByKey(hsm);
    if (hsmSeq == NULL) {
        return false;
    }
    if (hsmSeq->GetValue() != seqToMatch) {
        return false;
    }
    // Clear entry once matched.
    if (autoClear) {
        *hsmSeq = m_outHsmSeqMap.GetUnusedKv();
    }
    return true;
}

} // namespace FW

