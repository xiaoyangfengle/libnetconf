/**
 * \file session.h
 * \author Radek Krejci <rkrejci@cesnet.cz>
 * \brief Functions to handle NETCONF sessions.
 *
 * Copyright (C) 2012 CESNET, z.s.p.o.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name of the Company nor the names of its contributors
 *    may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * ALTERNATIVELY, provided that this notice is retained in full, this
 * product may be distributed under the terms of the GNU General Public
 * License (GPL) version 2 or later, in which case the provisions
 * of the GPL apply INSTEAD OF those given above.
 *
 * This software is provided ``as is, and any express or implied
 * warranties, including, but not limited to, the implied warranties of
 * merchantability and fitness for a particular purpose are disclaimed.
 * In no event shall the company or contributors be liable for any
 * direct, indirect, incidental, special, exemplary, or consequential
 * damages (including, but not limited to, procurement of substitute
 * goods or services; loss of use, data, or profits; or business
 * interruption) however caused and on any theory of liability, whether
 * in contract, strict liability, or tort (including negligence or
 * otherwise) arising in any way out of the use of this software, even
 * if advised of the possibility of such damage.
 *
 */

#ifndef SESSION_H_
#define SESSION_H_

#include "ssh.h"
#include "netconf.h"

/**
 * @ingroup session
 * @brief Close NETCONF connection with the server and cleanup the session structure.
 *
 * Do not use given session structure after this call.
 *
 * @param[in] session Session to close.
 */
void nc_session_close (struct nc_session* session);

/**
 * @ingroup session
 * @brief Get NETCONF protocol version used in the given session.
 * @param[in] session NETCONF session structure
 * @return NETCONF protocol version, 0 for 1.0, 1 for 1.1
 */
int nc_session_get_version(const struct nc_session* session);

/**
 * @ingroup session
 * @brief Get input file descriptor to asynchronous control of input events.
 *
 * Caller must avoid direct reading from the returned file descriptor. It is
 * supposed to be used only by select, poll, epoll or an event library (e.g.
 * libevent).
 *
 * @param[in] session NETCONF session structure
 * @return Input file descriptor of the communication channel.
 */
int nc_session_get_eventfd(const struct nc_session* session);

/**
 * @ingroup session
 * @brief Get NETCONF session ID
 * @param[in] session NETCONF session structure
 * @return Copy of the string identifying NETCONF session. Caller is
 * supposed to free returned value;
 */
char* nc_session_get_id(const struct nc_session* session);

/**
 * @ingroup session
 * @brief Get NULL terminated list of capabilities associated with the session.
 *
 * Returned list is a copy of the original list associated with the session.
 * Caller is supposed to free all returned strings.
 *
 * @param[in] session NETCONF session structure
 * @return NETCONF capabilities structure containing capabilities associated
 * with the given session. NULL is returned on error.
 */
struct nc_cpblts* nc_session_get_cpblts(const struct nc_session* session);

/**
 * @ingroup session
 * @brief Create new NETCONF capabilities structure.
 * @param list NULL terminated list of capabilities strings to initially add
 * into the NETCONF capabilities structure.
 * @return Created NETCONF capabilities structure.
 */
struct nc_cpblts *nc_cpblts_new(char **list);

/**
 * @ingroup session
 * @brief Free NETCONF capabilities structure.
 * @param c Capabilities structure to free.
 */
void nc_cpblts_free(struct nc_cpblts *c);

/**
 * @ingroup session
 * @brief Add another one capability string into the NETCONF capabilities structure.
 * @param capabilities Current NETCONF capabilities structure.
 * @param capability_string Capability string to add.
 * @return 0 on success\n non-zero on error
 */
int nc_cpblts_add (struct nc_cpblts *capabilities, const char* capability_string);

/**
 * @ingroup session
 * @brief Remove specified capability string from the NETCONF capabilities structure.
 * @param capabilities Current NETCONF capabilities structure.
 * @param capability_string Capability string to remove.
 * @return 0 on success\n non-zero on error
 */
int nc_cpblts_remove (struct nc_cpblts *capabilities, const char* capability_string);

/**
 * @ingroup session
 * @brief Move NETCONF capabilities structure iterator to the beginning of the capability strings list.
 * @param c NETCONF capabilities structure to be iterated.
 */
void nc_cpblts_iter_start(struct nc_cpblts *c);

/**
 * @ingroup session
 * @brief Get the next capability string from the given NETCONF capabilities structure.
 *
 * To move iterator to the beginning of the capability strings list, use
 * nc_cpblts_iter_start().
 *
 * @param c NETCONF capabilities structure to be iterated.
 * @return Another capability string, NULL if all strings were already returned.
 */
char *nc_cpblts_iter_next(struct nc_cpblts *c);

/**
 * @ingroup session
 * @brief Get NULL terminated list of default capabilities supported by libnetconf.
 *
 * Caller is supposed to free all returned strings.
 *
 * @return NETCONF capabilities structure containing capabilities supported by
 * libnetconf.
 */
struct nc_cpblts *nc_session_get_cpblts_default();

/**
 * @ingroup rpc
 * @brief Send \<rpc\> request via specified NETCONF session.
 * @param[in] session NETCONF session to use.
 * @param[in] rpc RPC message to send.
 * @return 0 on error,\n message-id of sent message on success.
 */
nc_msgid nc_session_send_rpc (struct nc_session* session, nc_rpc *rpc);

/**
 * @ingroup reply
 * @brief Receive \<rpc-reply\> from the specified NETCONF session.
 * @param[in] session NETCONF session to use.
 * @param[out] reply Received \<rpc-reply\>
 * @return 0 on error,\n message-id of received message on success.
 */
nc_msgid nc_session_recv_reply (struct nc_session* session, nc_reply** reply);

/**
 * @ingroup rpc
 * @brief Send \<rpc\> and receive \<rpc-reply\> via the specified NETCONF session.
 * @param[in] session NETCONF session to use.
 * @param[in] rpc RPC message to send.
 * @return Received \<rpc-reply\>.
 */
nc_reply *nc_session_send_recv (struct nc_session* session, nc_rpc *rpc);

#endif /* SESSION_H_ */