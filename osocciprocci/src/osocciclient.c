/* ------------------------------------------------------------------- */
/*  ACCORDS PLATFORM                                                   */
/*  (C) 2011 by Iain James Marshall (Prologue) <ijm667@hotmail.com>    */
/* --------------------------------------------------------------------*/
/*  This is free software; you can redistribute it and/or modify it    */
/*  under the terms of the GNU Lesser General Public License as        */
/*  published by the Free Software Foundation; either version 2.1 of   */
/*  the License, or (at your option) any later version.                */
/*                                                                     */
/*  This software is distributed in the hope that it will be useful,   */
/*  but WITHOUT ANY WARRANTY; without even the implied warranty of     */
/*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU   */
/*  Lesser General Public License for more details.                    */
/*                                                                     */
/*  You should have received a copy of the GNU Lesser General Public   */
/*  License along with this software; if not, write to the Free        */
/*  Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA */
/*  02110-1301 USA, or see the FSF site: http://www.fsf.org.           */
/* --------------------------------------------------------------------*/
#ifndef	_occi_os_client_c
#define	_occi_os_client_c

#include "osocciclient.h"

struct	occi_os_configuration
{
	char *	authorization;
	char *	tls;
	char *	agent;
	char *	tenant;
	char *	user;
	char *	password;
	char *	host;
	int	port;
};

private struct occi_os_configuration OcciConfig = 
{
	(char *) 0,
	(char *) 0,
	(char *) 0,
	(char *) 0,
	(char *) 0,
	0
};

public	int	liberate_occi_os_configuration(int status)
{
	if ( OcciConfig.host )
		OcciConfig.host = liberate( OcciConfig.host );
	if ( OcciConfig.tls )
		OcciConfig.tls = liberate( OcciConfig.tls );
	if ( OcciConfig.agent )
		OcciConfig.agent = liberate( OcciConfig.agent );
	if ( OcciConfig.user )
		OcciConfig.user = liberate( OcciConfig.user );
	if ( OcciConfig.password )
		OcciConfig.password = liberate( OcciConfig.password );
	if ( OcciConfig.tenant )
		OcciConfig.tenant = liberate( OcciConfig.tenant );
	if ( OcciConfig.authorization )
		OcciConfig.authorization = liberate( OcciConfig.authorization );
	OcciConfig.port = 0;
	return(status);
}

public	int	set_occi_os_configuration( char * host, int port, char * user, char * password, char * tenant, char * agent, char * tls )
{
	liberate_occi_os_configuration(0);

	if ( host )
		if (!( OcciConfig.host = allocate_string( host ) ))
			return( liberate_occi_os_configuration(27) );
	if ( user )
		if (!( OcciConfig.user = allocate_string( user ) ))
			return( liberate_occi_os_configuration(27) );
	if ( password )
		if (!( OcciConfig.password = allocate_string( password ) ))
			return( liberate_occi_os_configuration(27) );
	if ( tenant )
		if (!( OcciConfig.tenant = allocate_string( tenant ) ))
			return( liberate_occi_os_configuration(27) );
	if ( agent )
		if (!( OcciConfig.agent = allocate_string( agent ) ))
			return( liberate_occi_os_configuration(27) );
	if ( tls )
		if (!( OcciConfig.tls = allocate_string( tls ) ))
			return( liberate_occi_os_configuration(27) );

	OcciConfig.port = port;
	return( 0 );	
}

struct	rest_response * occi_os_failure( struct rest_header * hptr )
{
	hptr = liberate_rest_header( hptr );
	return(( struct rest_response * ) 0 );
}

struct	rest_header * occi_os_header(char* nptr, char * vptr)
{
	struct	rest_header * hptr=(struct rest_header *) 0;

	if (!( hptr = allocate_rest_header()))
		return( hptr );
	else if (!( hptr->name = allocate_string( nptr )))
		return( liberate_rest_header( hptr ) );
	else if (!( hptr->value = allocate_string( vptr )))
		return( liberate_rest_header( hptr ) );
	else	return( hptr );
}

struct	rest_header * occi_os_headers(char * category, char * content)
{
	struct	rest_header * root=(struct rest_header *) 0;
	struct	rest_header * foot=(struct rest_header *) 0;
	struct	rest_header * hptr=(struct rest_header *) 0;

	/* ---------------------------------------------- */
	/* first position the category header if provided */
	/* ---------------------------------------------- */
	if ( category )
	{
		if (!( hptr = occi_os_header( "Category", category ) ))
			return( liberate_rest_header( root ) );
		else if (!( hptr->previous = foot ))
			root = hptr;
		else	hptr->previous->next = hptr;
		foot = hptr;
	}

	/* ---------------------------------------------- */
	/* the add in the content type header if provided */
	/* ---------------------------------------------- */
	if ( content )
	{
		if (!( hptr = occi_os_header( "Content-Type", content ) ))
			return( liberate_rest_header( root ) );
		else if (!( hptr->previous = foot ))
			root = hptr;
		else	hptr->previous->next = hptr;
		foot = hptr;
	}

	/* ---------------------------------------------- */
	/* then the authorization header if its available */
	/* ---------------------------------------------- */
	if ( OcciConfig.authorization )
	{
		if (!( hptr = occi_os_header( "X-Auth-Tenant-Id", OcciConfig.authorization ) ))
			return( liberate_rest_header( root ) );
		else if (!( hptr->previous = foot ))
			root = hptr;
		else	hptr->previous->next = hptr;
		foot = hptr;
	}

	/* ---------------------------------------------- */
	/* then the subscription  header if its available */
	/* ---------------------------------------------- */
	if ( OcciConfig.tenant )
	{
		if (!( hptr = occi_os_header( "X-Auth-Token", OcciConfig.tenant ) ))
			return( liberate_rest_header( root ) );
		else if (!( hptr->previous = foot ))
			root = hptr;
		else	hptr->previous->next = hptr;
		foot = hptr;
	}

	/* ---------------------------------------------- */
	/* then the user logname  header if its available */
	/* ---------------------------------------------- */
	if ( OcciConfig.user )
	{
		if (!( hptr = occi_os_header( "X-Auth-User", OcciConfig.user ) ))
			return( liberate_rest_header( root ) );
		else if (!( hptr->previous = foot ))
			root = hptr;
		else	hptr->previous->next = hptr;
		foot = hptr;
	}
	/* ---------------------------------------------- */
	/* then the user agent    header if its available */
	/* ---------------------------------------------- */
	if ( OcciConfig.agent )
	{
		if (!( hptr = occi_os_header( "User-Agent", OcciConfig.agent ) ))
			return( liberate_rest_header( root ) );
		else if (!( hptr->previous = foot ))
			root = hptr;
		else	hptr->previous->next = hptr;
		foot = hptr;
	}

	return( root );
}

private	struct rest_header * occi_os_compute_headers()
{
	char	buffer[1024];
	sprintf(buffer, "compute; scheme=%chttp://schemas.ogf.org/occi/infrastructure#%c; class=%ckind%c",
			0x0022,0x0022,0x0022,0x0022 );
	return( occi_os_headers( buffer, "text/occi" ) );
}

private	struct rest_header * occi_os_compute_action_headers(char * nptr)
{
	char	buffer[1024];
	sprintf(buffer, "%s; scheme=%chttp://schemas.ogf.org/occi/infrastructure/compute/action%c; class=%caction%c",
			nptr, 0x0022,0x0022,0x0022,0x0022 );
	return( occi_os_headers( buffer, "text/occi" ) );
}

private	struct rest_header * occi_os_network_headers()
{
	char	buffer[1024];
	sprintf(buffer, "network; scheme=%chttp://schemas.ogf.org/occi/infrastructure#%c; class=%ckind%c",
			0x0022,0x0022,0x0022,0x0022 );
	return( occi_os_headers( buffer, "text/occi" ) );
}

private	struct rest_header * occi_os_network_interface_headers()
{
	char	buffer[1024];
	sprintf(buffer, "networkinterface; scheme=%chttp://schemas.ogf.org/occi/infrastructure#%c; class=%ckind%c",
			0x0022,0x0022,0x0022,0x0022 );
	return( occi_os_headers( buffer, "text/occi" ) );
}

private	char *	occi_os_category_url( char * term )
{
	char buffer[1024];
	sprintf(buffer,"%s:%u%s",OcciConfig.host,OcciConfig.port,term);
	return( allocate_string( buffer ) );
}

private	char *	occi_os_instance_url( char * term, char * id )
{
	char buffer[1024];
	sprintf(buffer,"%s:%u%s%s",OcciConfig.host,OcciConfig.port,term,id);
	return( allocate_string( buffer ) );
}

private	char *	occi_os_action_url( char * term, char * id, char * action )
{
	char buffer[1024];
	sprintf(buffer,"%s:%u%s%s?action=%s",OcciConfig.host,OcciConfig.port,term,id,action);
	return( allocate_string( buffer ) );
}

private	struct	rest_header * occi_os_add_mixin( struct rest_header * root, char * mixin, char * schema )
{
	struct	rest_header * hptr;
	struct	rest_header * foot;
	char buffer[1024];

	if ((foot = root) != (struct rest_header *) 0)
		while ( foot->next )
			foot = foot->next;

	sprintf(buffer, "%s; scheme=%c%s%c; class=%cmixin%", mixin, 0x0022,schema,0x0022,0x0022,0x0022 );
	if (!( hptr = occi_os_header( "Categroy", buffer ) ))
		return( liberate_rest_header( root ) );
	else if (!( hptr->previous = foot ))
		root = hptr;
	else 	hptr->previous->next = hptr;
	foot = hptr;
	return( root );
}

private	struct	rest_header * occi_os_add_attribute( struct rest_header * root, char * name, char * value )
{
	struct	rest_header * hptr;
	struct	rest_header * foot;
	char buffer[1024];

	if ((foot = root) != (struct rest_header *) 0)
		while ( foot->next )
			foot = foot->next;

	sprintf(buffer, "%s=%c%s%c", name, 0x0022,value,0x0022 );
	if (!( hptr = occi_os_header( "X-OCCI-Attribute", buffer ) ))
		return( liberate_rest_header( root ) );
	else if (!( hptr->previous = foot ))
		root = hptr;
	else 	hptr->previous->next = hptr;
	foot = hptr;
	return( root );
}

/* OCCI Capacities
curl -v -X GET localhost:8787/-/
-H 'X-Auth-Token: '$KID 
-H 'X-Auth-Tenant-Id: '$TEN_ID 
-H 'X-Auth-User: '$OS_USER 
*/

public struct	rest_response * occi_os_capacities()
{
	struct rest_header * hptr=(struct rest_header *) 0;
	char * url;
	if (!( hptr = occi_os_headers((char *) 0, (char *) 0)))
		return( occi_os_failure( hptr ));
	else if (!( url = occi_os_category_url( "/-/" ) ))
		return( occi_os_failure( liberate_rest_header( hptr ) ));
	else	return( rest_client_get_request( url, OcciConfig.tls, OcciConfig.agent, hptr ) );
}

/* CREATE COMPUTE instance
curl -v -X POST localhost:8787/compute/ 
-H 'Category: compute; scheme="http://schemas.ogf.org/occi/infrastructure#"; class="kind"' 
-H 'Content-Type: text/occi' 
-H 'X-Auth-Token: '$KID 
-H 'X-Auth-Tenant-Id: '$TEN_ID 
-H 'X-Auth-User: '$OS_USER 
-H 'Category: itsy; scheme="http://schemas.openstack.org/template/resource#"; class="mixin"' 
-H 'Category: cirros-0.3.0-x86_64-blank; scheme="http://schemas.openstack.org/template/os#"; class="mixin"'
*/

public struct	rest_response * create_occi_os_compute(char * machine, char * system)
{
	struct rest_header * hptr=(struct rest_header *) 0;
	char * url;
	if (!( hptr = occi_os_headers((char *) 0, (char *) 0)))
	if (!( hptr = occi_os_compute_headers()))
		return( occi_os_failure( hptr ) );
	else if (!( url = occi_os_category_url( "/compute/" ) ))
		return( occi_os_failure( liberate_rest_header( hptr )) );
	else if (!( hptr = occi_os_add_mixin( hptr, system, "http://schemas.openstack.org/template/os" )))
		return( occi_os_failure( hptr ) );
	else if (!( hptr = occi_os_add_mixin( hptr, machine, "http://schemas.openstack.org/template/resource" )))
		return( occi_os_failure( hptr ) );
	else	return( rest_client_post_request( url, OcciConfig.tls, OcciConfig.agent, (char *) 0, hptr ) );
}

/* STOP COMPUTE instance
curl -v -X POST "localhost:8787/compute/$VM?action=stop" 
-H 'Content-Type: text/occi' 
-H 'X-Auth-Token: '$KID 
-H 'X-Auth-Tenant-Id: '$TEN_ID 
-H 'X-Auth-User: '$OS_USER 
-H 'Category: stop; scheme="http://schemas.ogf.org/occi/infrastructure/compute/action#"; class="action"'
*/

public struct	rest_response * stop_occi_os_compute(char * vm)
{
	struct rest_header * hptr=(struct rest_header *) 0;
	char * url;
	if (!( hptr = occi_os_compute_action_headers("stop")))
		return( occi_os_failure( hptr ));
	else if (!( url = occi_os_action_url( "/compute/",vm,"stop" ) ))
		return( occi_os_failure( liberate_rest_header( hptr )) );
	else	return( rest_client_post_request( url, OcciConfig.tls, OcciConfig.agent, (char *) 0, hptr ) );
}

/* START COMPUTE instance
curl -v -X POST localhost:8787/compute/$VM?action=start 
-H 'Content-Type: text/occi' 
-H 'X-Auth-Token: '$KID 
-H 'X-Auth-Tenant-Id: '$TEN_ID 
-H 'X-Auth-User: '$OS_USER 
-H 'Category: start; scheme="http://schemas.ogf.org/occi/infrastructure/compute/action#"; class="action"'
*/

public struct	rest_response * start_occi_os_compute(char * vm)
{
	struct rest_header * hptr=(struct rest_header *) 0;
	char * url;
	if (!( hptr = occi_os_compute_action_headers("start")))
		return( occi_os_failure( hptr ) );
	else if (!( url = occi_os_action_url( "/compute/",vm,"start" ) ))
		return( occi_os_failure( liberate_rest_header( hptr ) ) );
	else	return( rest_client_post_request( url, OcciConfig.tls, OcciConfig.agent, (char *) 0, hptr ) );
}

/* DELETE COMPUTE instance
curl -v -X DELETE localhost:8787/compute/$VM 
-H 'Content-Type: text/occi' 
-H 'X-Auth-Token: '$KID 
-H 'X-Auth-Tenant-Id: '$TEN_ID 
-H 'X-Auth-User: '$OS_USER
*/


public struct	rest_response * delete_occi_os_compute(char * vm)
{
	struct rest_header * hptr=(struct rest_header *) 0;
	char * url;
	if (!( hptr = occi_os_compute_headers()))
		return( occi_os_failure( hptr ));
	else if (!( url = occi_os_instance_url( "/compute/", vm ) ))
		return( occi_os_failure( liberate_rest_header( hptr ) ));
	else	return( rest_client_delete_request( url, OcciConfig.tls, OcciConfig.agent, hptr ) );
}

/* CREATE NETWORK
curl -v -X POST localhost:8787/network/ 
-H 'Content-Type: text/occi' 
-H 'X-Auth-Token: '$KID 
-H 'X-Auth-Tenant-Id: '$TEN_ID 
-H 'X-Auth-User: '$OS_USER 
-H 'Category: network; scheme="http://schemas.ogf.org/occi/infrastructure#"; class="kind"' 
-H 'occi.network.label="www"'
*/

public struct	rest_response * create_occi_os_network(char * label)
{
	struct rest_header * hptr=(struct rest_header *) 0;
	char * url;
	if (!( hptr = occi_os_network_headers()))
		return( occi_os_failure( hptr ));
	else if (!( url = occi_os_category_url( "/network/" ) ))
		return( occi_os_failure( liberate_rest_header( hptr ) ));
	else if (!( hptr = occi_os_add_attribute( hptr, "occi.network.label", label ) ))
		return( occi_os_failure( hptr ));
	else	return( rest_client_post_request( url, OcciConfig.tls, OcciConfig.agent, (char *) 0, hptr ) );
}

/* ATTACH NETWORK to COMPUTE instance
curl -v -X POST localhost:8787/networklink/ 
-H 'Content-Type: text/occi' 
-H 'X-Auth-Token: '$KID 
-H 'X-Auth-Tenant-Id: '$TEN_ID 
-H 'X-Auth-User: '$OS_USER 
-H 'Category: networkinterface; scheme="http://schemas.ogf.org/occi/infrastructure#"; class="kind"' 
-H 'X-OCCI-Attribute: occi.core.source="http://localhost:8787/compute/'$VM'"' 
-H 'X-OCCI-Attribute: occi.core.target="http://localhost:8787/network/'$NET'"'
*/

public struct	rest_response * attach_occi_os_network(char * vm,char * nw)
{
	struct rest_header * hptr=(struct rest_header *) 0;
	char * url;
	if (!( hptr = occi_os_network_interface_headers()))
		return( occi_os_failure( hptr ));
	else if (!( url = occi_os_category_url( "/networklink/" ) ))
		return( occi_os_failure( liberate_rest_header( hptr ) ));
	else if (!( hptr = occi_os_add_attribute( hptr, "occi.core.source", vm ) ))
		return( occi_os_failure( hptr ));
	else if (!( hptr = occi_os_add_attribute( hptr, "occi.core.target", nw ) ))
		return( occi_os_failure( hptr ) );
	else	return( rest_client_post_request( url, OcciConfig.tls, OcciConfig.agent, (char *) 0, hptr ) );
}

/* DELETE NETWORK
curl -v -X DELETE localhost:8787/network/$NET_ID 
-H 'Content-Type: text/occi' 
-H 'X-Auth-Token: '$KID 
-H 'X-Auth-Tenant-Id: '$TEN_ID 
-H 'X-Auth-User: '$OS_USER
*/

public struct	rest_response * delete_occi_os_network(char * nw)
{
	struct rest_header * hptr=(struct rest_header *) 0;
	char * url;
	if (!( hptr = occi_os_network_headers()))
		return( occi_os_failure( hptr ));
	else if (!( url = occi_os_instance_url( "/network/",nw ) ))
		return( occi_os_failure( liberate_rest_header( hptr ) ) );
 	else	return( rest_client_delete_request( url, OcciConfig.tls, OcciConfig.agent, hptr ) );
}

#endif /* _occi_os_client_c */
	/* ------------------ */