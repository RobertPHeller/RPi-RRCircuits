/** \copyright
 * Copyright (c) 2020, Mike Dunston
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are  permitted provided that the following conditions are met:
 *
 *  - Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 *  - Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \file FactoryResetHelper.hxx
 *
 * Resets the node name and description based on the configured Node ID.
 *
 * @author Mike Dunston
 * @date 10 November 2020
 */

#ifndef FACTORY_RESET_HELPER_HXX_
#define FACTORY_RESET_HELPER_HXX_

#include "cdi.hxx"

#include <algorithm>
#include <stdint.h>
#include <executor/Notifiable.hxx>
#include <utils/ConfigUpdateListener.hxx>
#include <utils/format_utils.hxx>

namespace esp32pwmhalfsiding
{

// when the io board starts up the first time the config is blank and needs to
// be reset to factory settings.
class FactoryResetHelper : public DefaultConfigUpdateListener
{
public:
    FactoryResetHelper()
    {
    }

    UpdateAction apply_configuration(int fd, bool initial_load,
                                     BarrierNotifiable *done) override;
    void factory_reset(int fd) override;
};

} // namespace esp32pwmhalfsiding

#endif // FACTORY_RESET_HELPER_HXX_