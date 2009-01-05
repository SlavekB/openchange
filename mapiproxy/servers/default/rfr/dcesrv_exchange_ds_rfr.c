/*
   MAPI Proxy - Exchange RFR Server

   OpenChange Project

   Copyright (C) Julien Kerihuel 2009

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
   \file dcesrv_exchange_rfr.c

   \brief OpenChange RFR Server implementation
 */

#include "mapiproxy/dcesrv_mapiproxy.h"
#include "mapiproxy/libmapiproxy.h"
#include "dcesrv_exchange_ds_rfr.h"
#include <util/debug.h>


/**
   \details exchange_ds_rfr RfrGetNewDSA (0x0) function

   \param dce_call pointer to the session context
   \param mem_ctx pointer to the memory context
   \param r pointer to the RfrGetNewDSA request data

   \return MAPI_E_SUCCESS on success
 */
static enum MAPISTATUS dcesrv_RfrGetNewDSA(struct dcesrv_call_state *dce_call,
					   TALLOC_CTX *mem_ctx,
					   struct RfrGetNewDSA *r)
{
	DEBUG(3, ("exchange_ds_rfr: RfrGetNewDSA (0x0) not implemented\n"));
	DCESRV_FAULT(DCERPC_FAULT_OP_RNG_ERROR);
}


/**
   \details exchange_ds_rrf RfrGetFQDNFromLegacyDN (0x1) function

   \param dce_call pointer to the session context
   \param mem_ctx pointer to the memory context
   \param r pointer to the RfrGetFQDNFromLegacyDN request data

   \return MAPI_E_SUCCESS on success
 */
static enum MAPISTATUS dcesrv_RfrGetFQDNFromLegacyDN(struct dcesrv_call_state *dce_call,
						     TALLOC_CTX *mem_ctx,
						     struct RfrGetFQDNFromLegacyDN *r)
{
	DEBUG(3, ("exchange_ds_rfr: RfrGetFQDNFromLegacyDN (0x1) not implemented\n"));
	DCESRV_FAULT(DCERPC_FAULT_OP_RNG_ERROR);
}


/**
   \details Dispatch incoming RFR call to the correct OpenChange server function

   \param dce_call pointer to the session context
   \param mem_ctx pointer to the memory context
   \param r generic pointer on RFR data
   \param mapiproxy pointer to the mapiproxy structure controlling
   mapiproxy behavior

   \return NT_STATUS_OK
 */
static NTSTATUS dcesrv_exchange_ds_rfr_dispatch(struct dcesrv_call_state *dce_call,
						TALLOC_CTX *mem_ctx,
						void *r, struct mapiproxy *mapiproxy)
{
	enum MAPISTATUS				retval;
	const struct ndr_interface_table	*table;
	uint16_t				opnum;

	table = (const struct ndr_interface_table *) dce_call->context->iface->private;
	opnum = dce_call->pkt.u.request.opnum;

	/* Sanity checks */
	if (!table) return NT_STATUS_UNSUCCESSFUL;
	if (table->name && strcmp(table->name, NDR_EXCHANGE_DS_RFR_NAME)) return NT_STATUS_UNSUCCESSFUL;

	switch (opnum) {
	case NDR_RFRGETNEWDSA:
		retval = dcesrv_RfrGetNewDSA(dce_call, mem_ctx, (struct RfrGetNewDSA *)r);
		break;
	case NDR_RFRGETFQDNFROMLEGACYDN:
		retval = dcesrv_RfrGetFQDNFromLegacyDN(dce_call, mem_ctx, (struct RfrGetFQDNFromLegacyDN *)r);
		break;
	}


	return NT_STATUS_OK;
}


/**
   \details Initialize the RFR OpenChange server

   \param dce_ctx pointer to the server context

   \return NT_STATUS_OK on success
 */
static NTSTATUS dcesrv_exchange_ds_rfr_init(struct dcesrv_context *dce_ctx)
{
	return NT_STATUS_OK;
}


/**
   \details Terminate the RFR connection

   \param server_id reference to the server identifier structure
   \param context_id the connection context identifier

   \return NT_STATUS_OK on success
 */
static NTSTATUS dcesrv_exchange_ds_rfr_unbind(struct server_id server_id, 
					      uint32_t context_id)
{
	return NT_STATUS_OK;
}


/**
   \details Entry point for the default OpenChange RFR server

   \return NT_STATUS_OK on success, otherwise NTSTATUS error
 */
NTSTATUS samba_init_module(void)
{
	struct mapiproxy_module	server;
	NTSTATUS		ret;

	/* Fill in our name */
	server.name = "exchange_ds_rfr";
	server.status = MAPIPROXY_DEFAULT;
	server.description = "OpenChange RFR server";
	server.endpoint = "exchange_ds_rfr";

	/* Fill in all the operations */
	server.init = dcesrv_exchange_ds_rfr_init;
	server.unbind = dcesrv_exchange_ds_rfr_unbind;
	server.dispatch = dcesrv_exchange_ds_rfr_dispatch;
	server.push = NULL;
	server.pull = NULL;
	server.ndr_pull = NULL;

	/* Register ourselves with the MAPIPROXY server subsystem */
	ret = mapiproxy_server_register(&server);
	if (!NT_STATUS_IS_OK(ret)) {
		DEBUG(0, ("Failed to register the 'exchange_ds_rfr' default mapiproxy server!\n"));
		return ret;
	}

	return ret;
}