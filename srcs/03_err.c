/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   03_err.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qliso <qliso@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 16:18:02 by qliso             #+#    #+#             */
/*   Updated: 2025/03/26 17:44:51 by qliso            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

const char	*get_err_msg(t_errnum err)
{
	static const char	*msg[] = {
		NOERR_M, EARGS_M, EDIR_M, EOPEN_M, EFILEEXT_M, EMALLOC_M,
		EWRONGCHAR_M, EEMPTYFILE_M, EMISSINGTEX_M, EMISSINGTEXN_M,
		EMISSINGTEXS_M, EMISSINGTEXW_M, EMISSINGTEXE_M, EMISSINGCOLC_M,
		EMISSINGCOLF_M, EDOUBLETEX_M, EPATHTEX_M, ERGBFORMAT_M, ERGBVAL_M,
		EEMPTYMAP_M, EMAPTOOSMALL_M, ECHAREDGE_M, ECHARMAP_M, EVOIDONSIDE_M,
		EDOUBLEORIENT_M, EORIENTONSIDE_M, EMISSINGORIENT_M, EPLAYERNOSPACE_M,
		EMISSINGWALLS_M, EMLXINIT_M, EMLXWIN_M, EMLXIMG_M, ELOADTEXTURE_M, 
		EGETIMGADDR_M, EUNKNOWN_M
	};
	if (err >= (sizeof(msg) / sizeof(char *)) || err < 0)
		return (msg[EUNKNOWN]);
	return (msg[err]);
}

int perror_c3d(t_errnum errnum)
{
	const char	*msg = get_err_msg(errnum);

    ft_putstr_fd("CUB3D ERROR", STDERR_FILENO);
    if (msg)
    {
        ft_putstr_fd(": ", STDERR_FILENO);
        ft_putendl_fd((char *)msg, STDERR_FILENO);
    }
    return (errnum);
}
