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

#ifndef HSM_ID_H
#define HSM_ID_H

#include "fw_def.h"

namespace APP {

enum {
    SYSTEM = FW::HSM_START,
    UART_ACT,
    UART_ACT_COUNT = 2,
        UART1_ACT = UART_ACT,
        UART6_ACT,
    UART_ACT_LAST = UART_ACT + UART_ACT_COUNT - 1,
    UART_IN,
    UART_IN_COUNT = UART_ACT_COUNT,
        UART1_IN = UART_IN,
        UART6_IN,
    UART_IN_LAST = UART_IN + UART_IN_COUNT - 1,
    UART_OUT,
    UART_OUT_COUNT = UART_ACT_COUNT,
        UART1_OUT = UART_OUT,
        UART6_OUT,
    UART_OUT_LAST = UART_OUT + UART_OUT_COUNT - 1,
    CONSOLE,
    SAMPLE,
    SAMPLE_REG,
    SAMPLE_REG_COUNT = 4,
        SAMPLE_REG0 = SAMPLE_REG,     // Optional.
        SAMPLE_REG1,                  // Optional.
        SAMPLE_REG2,                  // Optional.
        SAMPLE_REG3,                  // Optional.
    SAMPLE_REG_LAST = SAMPLE_REG + SAMPLE_REG_COUNT - 1,
    HSM_COUNT
};

// Higher value corresponds to higher priority.
// The maximum priority is defined in qf_port.h as QF_MAX_ACTIVE (32)
enum
{
    PRIO_UART6_ACT  = 30,
    PRIO_UART1_ACT  = 29,
    PRIO_CONSOLE    = 28,
    PRIO_SYSTEM     = 26,
    PRIO_SAMPLE     = 5
};

} // namespace APP

#endif // HSM_ID_H
