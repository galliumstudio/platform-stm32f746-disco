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

#include "hsm_id.h"
#include "fw_log.h"
#include "fw_assert.h"
#include "SampleRegInterface.h"
#include "SampleReg.h"

FW_DEFINE_THIS_FILE("SampleReg.cpp")

namespace APP {

static char const * const timerEvtName[] = {
    "STATE_TIMER",
};

static char const * const internalEvtName[] = {
    "DONE",
};

static char const * const interfaceEvtName[] = {
    "SAMPLE_REG_START_REQ",
    "SAMPLE_REG_START_CFM",
    "SAMPLE_REG_STOP_REQ",
    "SAMPLE_REG_STOP_CFM",
};

static char const * const hsmName[] = {
    "SAMPLE_REG0",
    "SAMPLE_REG1",
    "SAMPLE_REG2",
    "SAMPLE_REG3"
};

static char const * GetName(HsmId id) {
    uint16_t inst = id - SAMPLE_REG;
    FW_ASSERT(inst < ARRAY_COUNT(hsmName));
    return hsmName[inst];
}

static HsmId &GetCurrId() {
    static HsmId hsmId = SAMPLE_REG;
    FW_ASSERT(hsmId <= SAMPLE_REG_LAST);
    return hsmId;
}

static void IncCurrId() {
    HsmId &currId = GetCurrId();
    ++currId;
    FW_ASSERT(currId > 0);
}

static uint16_t GetInst(HsmId id) {
    uint16_t inst = id - SAMPLE_REG;
    FW_ASSERT(inst < SAMPLE_REG_COUNT);
    return inst;
}

SampleReg::SampleReg() :
    Region((QStateHandler)&SampleReg::InitialPseudoState, GetCurrId(), GetName(GetCurrId()),
           timerEvtName, ARRAY_COUNT(timerEvtName),
           internalEvtName, ARRAY_COUNT(internalEvtName),
           interfaceEvtName, ARRAY_COUNT(interfaceEvtName)),
    m_stateTimer(this->GetHsm().GetId(), STATE_TIMER) {
    IncCurrId();
}

QState SampleReg::InitialPseudoState(SampleReg * const me, QEvt const * const e) {
    (void)e;
    return Q_TRAN(&SampleReg::Root);
}

QState SampleReg::Root(SampleReg * const me, QEvt const * const e) {
    QState status;
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            EVENT(e);
            status = Q_HANDLED();
            break;
        }
        case Q_EXIT_SIG: {
            EVENT(e);
            status = Q_HANDLED();
            break;
        }
        case Q_INIT_SIG: {
            status = Q_TRAN(&SampleReg::Stopped);
            break;
        }
        case SAMPLE_REG_START_REQ: {
            EVENT(e);
            Evt const &req = EVT_CAST(*e);
            Evt *evt = new SampleRegStartCfm(req.GetFrom(), GET_ID(), req.GetSeq(), ERROR_STATE, GET_ID());
            Fw::Post(evt);
            status = Q_HANDLED();
            break;
        }
        default: {
            status = Q_SUPER(&QHsm::top);
            break;
        }
    }
    return status;
}

QState SampleReg::Stopped(SampleReg * const me, QEvt const * const e) {
    QState status;
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            EVENT(e);
            status = Q_HANDLED();
            break;
        }
        case Q_EXIT_SIG: {
            EVENT(e);
            status = Q_HANDLED();
            break;
        }
        case SAMPLE_REG_STOP_REQ: {
            EVENT(e);
            Evt const &req = EVT_CAST(*e);
            Evt *evt = new SampleRegStopCfm(req.GetFrom(), GET_ID(), req.GetSeq(), ERROR_SUCCESS);
            Fw::Post(evt);
            status = Q_HANDLED();
            break;
        }
        case SAMPLE_REG_START_REQ: {
            EVENT(e);
            Evt const &req = EVT_CAST(*e);
            Evt *evt = new SampleRegStartCfm(req.GetFrom(), GET_ID(), req.GetSeq(), ERROR_SUCCESS);
            Fw::Post(evt);
            status = Q_TRAN(&SampleReg::Started);
            break;
        }
        default: {
            status = Q_SUPER(&SampleReg::Root);
            break;
        }
    }
    return status;
}

QState SampleReg::Started(SampleReg * const me, QEvt const * const e) {
    QState status;
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            EVENT(e);
            LOG("Instance = %d", GetInst(GET_ID()));
            status = Q_HANDLED();
            break;
        }
        case Q_EXIT_SIG: {
            EVENT(e);
            status = Q_HANDLED();
            break;
        }
        case SAMPLE_REG_STOP_REQ: {
            EVENT(e);
            Evt const &req = EVT_CAST(*e);
            Evt *evt = new SampleRegStopCfm(req.GetFrom(), GET_ID(), req.GetSeq(), ERROR_SUCCESS);
            Fw::Post(evt);
            status = Q_TRAN(&SampleReg::Stopped);
            break;
        }
        default: {
            status = Q_SUPER(&SampleReg::Root);
            break;
        }
    }
    return status;
}

/*
QState SampleReg::MyState(SampleReg * const me, QEvt const * const e) {
    QState status;
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            EVENT(e);
            status = Q_HANDLED();
            break;
        }
        case Q_EXIT_SIG: {
            EVENT(e);
            status = Q_HANDLED();
            break;
        }
        case Q_INIT_SIG: {
            status = Q_TRAN(&SampleReg::SubState);
            break;
        }
        default: {
            status = Q_SUPER(&SampleReg::SuperState);
            break;
        }
    }
    return status;
}
*/

} // namespace APP

