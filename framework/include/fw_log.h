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

#ifndef FW_LOG_H
#define FW_LOG_H

#include "fw_def.h"
#include "fw_error.h"
#include "fw_map.h"
#include "fw_pipe.h"
#include "fw_bitset.h"
#include "fw_assert.h"

#define FW_LOG_ASSERT(t_) ((t_) ? (void)0 : Q_onAssert("fw_log.h", (int_t)__LINE__))

namespace FW {

#define PRINT(format_, ...)      Log::Print(format_, ## __VA_ARGS__)
// The following macros can only be used within an HSM. Newline is automatically appended.
#define EVENT(e_)                Log::Event(Log::TYPE_LOG, me->GetHsm().GetId(), e_, __FUNCTION__);
#define INFO(format_, ...)       Log::Debug(Log::TYPE_INFO, me->GetHsm().GetId(), format_, ## __VA_ARGS__)
#define LOG(format_, ...)        Log::Debug(Log::TYPE_LOG, me->GetHsm().GetId(), format_, ## __VA_ARGS__)
#define CRITICAL(format_, ...)   Log::Debug(Log::TYPE_CRITICAL, me->GetHsm().GetId(), format_, ## __VA_ARGS__)
#define WARNING(format_, ...)    Log::Debug(Log::TYPE_WARNING, me->GetHsm().GetId(), format_, ## __VA_ARGS__)
#define ERROR(format_, ...)      Log::Debug(Log::TYPE_ERROR, me->GetHsm().GetId(), format_, ## __VA_ARGS__)

#define BUF_PRINT(buf_, len_, unit_, label_)    Log::BufPrint(buf_, len_, unit_, label_)
// The following macros can only be used within an HSM. Newline is automatically appended.
#define BUF_INFO(buf_, len_, unit_, label_)     Log::BufDebug(Log::TYPE_INFO, me->GetHsm().GetId(), buf_, len_, unit_, label_)
#define BUF_LOG(buf_, len_, unit_, label_)      Log::BufDebug(Log::TYPE_LOG, me->GetHsm().GetId(), buf_, len_, unit_, label_)
#define BUF_CRITICAL(buf_, len_, unit_, label_) Log::BufDebug(Log::TYPE_CRITICAL, me->GetHsm().GetId(), buf_, len_, unit_, label_)
#define BUF_WARNING(buf_, len_, unit_, label_)  Log::BufDebug(Log::TYPE_WARNING, me->GetHsm().GetId(), buf_, len_, unit_, label_)
#define BUF_ERROR(buf_, len_, unit_, label_)    Log::BufDebug(Log::TYPE_ERROR, me->GetHsm().GetId(), buf_, len_, unit_, label_)

class Log {
public:
    // Types are listed in decreasing order of criticality.
    // Order must match type names defined in m_typeName.
    enum Type {
        TYPE_ERROR,
        TYPE_WARNING,
        TYPE_CRITICAL,
        TYPE_LOG,
        TYPE_INFO,
        NUM_TYPE
    };

    enum {
        MAX_VERBOSITY = 5,      // 0-5. The higher the value the more verbose it is.
                                // 0 - All debug out disabled.
                                // 1 - Shows ERROR (Type 0).
                                // 2 - Shows ERROR, WARNING (Type 0-1).
                                // 3 - Shows ERROR, WARNING, CRITICAL (Type 0-2).
                                // 4 - Shows ERROR, WARNING, CRITICAL, LOG (Type 0-3).
                                // 5 - Shows ERROR, WARNING, CRITICAL, LOG, INFO (Type 0-4).
        DEFAULT_VERBOSITY = 0
    };

    // Add and remove output device interface.
    static void AddInterface(HsmId id, Fifo *fifo, QP::QSignal sig);
    static void RemoveInterface(HsmId id);

    static void Write(char const *buf, uint32_t len);
    static uint32_t Print(char const *format, ...);
    static void PrintItem(uint32_t &index, uint32_t minWidth, uint32_t itemPerLine, char const *format, ...);
    static void Event(Type type, HsmId id, QP::QEvt const *e, char const *func);
    static void Debug(Type type, HsmId id, char const *format, ...);
    static void BufPrint(uint8_t const *dataBuf, uint32_t dataLen, uint8_t align = 1, uint32_t label = 0);
    static void BufDebug(Type type, HsmId id, uint8_t const *dataBuf, uint32_t dataLen, uint8_t align = 1, uint32_t label = 0);

    static uint8_t GetVerbosity() { return m_verbosity; }
    static void SetVerbosity(uint8_t v) {
        FW_LOG_ASSERT(v <= MAX_VERBOSITY);
        m_verbosity = v;
    }
    static void On(HsmId id);
    static void Off(HsmId id);
    static void OnAll();
    static void OffAll();

    static char const *GetEvtName(QP::QSignal sig);
    static char const *GetHsmName(HsmId id);
    static char const *GetTypeName(Type type);
private:
    static bool IsOutput(Type type, HsmId id);
    static void BufLinePrint(uint8_t const *lineBuf, uint32_t lineLen, uint8_t unit, uint32_t lineLabel);

    enum {
        BUF_LEN = 160,
        BYTE_PER_LINE = 16
    };

    class Inf {
    public:
        Inf(Fifo *fifo = NULL, QP::QSignal sig = 0) :
            m_fifo(fifo), m_sig(sig) {
        }
        Fifo *GetFifo() const { return m_fifo; }
        QP::QSignal GetSig() const { return m_sig; }
    private:
        Fifo *m_fifo;
        QP::QSignal m_sig;
        // Use built-in memberwise copy constructor and assignment operator.
    };

    typedef KeyValue<HsmId, Inf> HsmInf;
    typedef Map<HsmId, Inf> HsmInfMap;

    static uint8_t m_verbosity;
    static uint32_t m_onStor[ROUND_UP_DIV(MAX_HSM_COUNT, 32)];
    static Bitset m_on;
    static HsmInf m_hsmInfStor[MAX_HSM_COUNT];
    static HsmInfMap m_hsmInfMap;
    static char const * const m_typeName[NUM_TYPE];
    static char const m_truncatedError[];
};

} // namespace FW

#endif // FW_LOG_H
