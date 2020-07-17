/**
 * Copyright (c) 2020 Paul-Louis Ageneau
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#include "verifiedtlstransport.hpp"
#include "include.hpp"

#if RTC_ENABLE_WEBSOCKET

using std::shared_ptr;
using std::string;
using std::unique_ptr;
using std::weak_ptr;

namespace rtc {

#if USE_GNUTLS

VerifiedTlsTransport::VerifiedTlsTransport(shared_ptr<TcpTransport> lower, string host,
                                           state_callback callback)
    : TlsTransport(std::move(lower), std::move(host), std::move(callback)) {}

VerifiedTlsTransport::~VerifiedTlsTransport() {}

void VerifiedTlsTransport::postCreation() {
	PLOG_DEBUG << "Setting up TLS certificate verification";
	gnutls_session_set_verify_cert(mSession, mHost.c_str(), 0);
}

void VerifiedTlsTransport::postHandshake() {
	// Nothing to do
}

#else // USE_GNUTLS==0

VerifiedTlsTransport::VerifiedTlsTransport(shared_ptr<TcpTransport> lower, string host,
                                           state_callback callback)
    : TlsTransport(std::move(lower), std::move(host), std::move(callback)) {}

VerifiedTlsTransport::~VerifiedTlsTransport() {}

void VerifiedTlsTransport::postCreation() {
	PLOG_DEBUG << "Setting up TLS certificate verification";
	SSL_set_verify(mSsl, SSL_VERIFY_PEER, NULL);
	SSL_set_verify_depth(mSsl, 4);
}

void VerifiedTlsTransport::postHandshake() {
	// Nothing to do
}

#endif

} // namespace rtc

#endif
