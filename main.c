#include <argvp.h>
#include <core.h>
#include <err.h>
#include <debug.h>
#include <fileio.h>
#include <mem.h>
#include <print.h>
#include <prompt.h>
#include <str.h>

/* ----- PROJECT TYPE OPTION CHARS ----- */

#define PROJ_TYPE_APP_OPT       '1'
#define PROJ_TYPE_LIB_OPT       '2'
#define PROJ_TYPE_HEADLIB_OPT   '3'

/* ----- OTHER CONSTANTS ----- */

#define HELP_TXT_PATH           "/usr/local/etc/mkmk/HELP.txt"
#define OPTSTR                  "hv"
#define PROJ_NAME_BUF_SZ        64
#define PROJ_DESC_BUF_SZ        256
#define PROJ_VER_BUF_SZ         64
#define TMPL_PATH_APP           "/usr/local/etc/mkmk/app.make"
#define TMPL_PATH_LIB           "/usr/local/etc/mkmk/lib.make"
#define TMPL_PATH_HEADLIB       "/usr/local/etc/mkmk/headlib.make"
#define VER_TXT_PATH            "/usr/local/etc/mkmk/VERSION.txt"


/* ----- MAIN FUNCTION ----- */

I16 main(I16 argc, Ch **argv) {

    // Initialization
    Argvp                   argvp;
    Err                     err;
    Ch                      *ver_file_txt;
    Ch                      *help_file_txt;
    Ch                      *tmpl_txt;
    Ch                      proj_name[PROJ_NAME_BUF_SZ];
    Ch                      proj_desc[PROJ_DESC_BUF_SZ];
    Ch                      proj_ver[PROJ_VER_BUF_SZ];
    Ch                      in_char;
    ver_file_txt            = NIL;
    help_file_txt           = NIL;
    tmpl_txt                = NIL;
    in_char                 = '\0';
    proj_name[0]            = '\0';
    proj_desc[0]            = '\0';
    proj_ver[0]             = '\0';
    init_argvp(&argvp); 
    init_err(&err);

    // Parse arguments and handle static output options (-h, -v, ...)
    ld_argvp(&argvp, argc, argv, OPTSTR, &err);
    if (is_err(&err))
        goto CLEANUP;
    if (argvp.pos_ct != 0) {
        THROW(&err, ErrCode_ARGV, "Expected 0 positionals, %d given", 
            argvp.pos_ct);
        goto CLEANUP;
    }
    if (get_argv_flag(&argvp, 'h')) {
        read_file_to_str(HELP_TXT_PATH, &help_file_txt, &err);
        if (is_err(&err))
            goto CLEANUP;
        print_fmt("\n%s\n", help_file_txt);
        goto CLEANUP;
    }
    if (get_argv_flag(&argvp, 'v')) {
        read_file_to_str(VER_TXT_PATH, &ver_file_txt, &err);
        if (is_err(&err))
            goto CLEANUP;
        print_fmt("\n%s\n", ver_file_txt);
        goto CLEANUP;
    }
    
    while (!is_in_str(in_char, "123"))
        in_char = prompt_ch("Project Type? (1=app; 2=lib; 3=header-only-lib) "
            ">>> ");
    prompt_ln("Project Name >>> ", proj_name, PROJ_NAME_BUF_SZ - 1);
    prompt_ln("Project Description >>> ", proj_desc, PROJ_DESC_BUF_SZ - 1);
    prompt_ln("Initial Version Number >>> ", proj_ver, PROJ_VER_BUF_SZ - 1);
    switch (in_char) {
        case '1':
        read_file_to_str(TMPL_PATH_APP, &tmpl_txt, &err);
        break;

        case '2':
        read_file_to_str(TMPL_PATH_LIB, &tmpl_txt, &err);
        break;

        case '3':
        read_file_to_str(TMPL_PATH_HEADLIB, &tmpl_txt, &err);
        break;

        default:
        ASSERT_UNREACH("%s", "Invalid project type given")
    }
    if (is_err(&err))
        goto CLEANUP;

    write_fmt_to_file(
        "Makefile",
        "# ----- PROJECT INFO ----- \n"
        "\n"
        "PROJ_NAME := %s\n"
        "PROJ_DESC := \"%s\"\n"
        "PROJ_VER := %s\n"
        "\n%s",
        &err, proj_name, proj_desc, proj_ver, tmpl_txt);
    if (is_err(&err))
        goto CLEANUP;

    print_ln("Successfully generated 'Makefile'!");

    CLEANUP:
    if (is_err(&err))
        warn(&err);
    free_mem(ver_file_txt);
    free_mem(help_file_txt);
    free_mem(tmpl_txt);

    return err.code;
    
}
