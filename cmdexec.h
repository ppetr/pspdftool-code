#ifndef _cmdexec
/**\file cmdexec.h
 * \brief Prikazovy interpret pro knihovnu.
 * Jedna se o implementaci jednoducheho prikazoveho interpretu.
 * Funkci cmd_exec se preda soubor s prikazy, ktere se maji aplikovat na predany seznam stranek.
 */


#define _cmdexec
#include "common.h"	
#include "vdoc.h"
#include "fileio.h"
/** \brief Funkce aplikuje prikazy ze souboru f na seznam stranek p_doc.
 * \param p_doc ukazatel na seznam stranek
 * \param f file descriptor na soubor s prikazy
 * \retval 0 OK
 * \retval -1 behem zpracovani se vyskytla chyba*/
int cmd_exec(page_list_head * p_doc, MYFILE * f);
/** \brief Funkce vypise informace o prikazech interpretu
 * \param f kam se budou prikazy vypisovat*/
void cmd_print_info(FILE *f);
#endif
