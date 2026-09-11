#include <stdio.h>
#include <stdlib.h>
#include "lib/parser.h"

/* Unicode box-drawing reference (UTF-8 terminal)
 * Light:   ─ │ ┌ ┐ └ ┘ ├ ┤ ┬ ┴ ┼
 * Heavy:   ━ ┃ ┏ ┓ ┗ ┛ ┣ ┫ ┳ ┻ ╋
 * Double:  ═ ║ ╔ ╗ ╚ ╝ ╠ ╣ ╦ ╩ ╬
 * Rounded: ╭ ╮ ╰ ╯
 * Blocks:  ░ ▒ ▓ █ ▀ ▄ ▌ ▐
 *
 * ┌──────────────┐  ╔══════════════╗
 * │ SHAMAANI     │  ║ SHAMAANI     ║
 * ├──────────────┤  ╠══════════════╣
 * │              │  ║              ║
 * └──────────────┘  ╚══════════════╝
*/

int main(int argc, char **argv) {
    if (argc > 2) {
        fprintf(stderr, "Käyttö: %s [aineisto.tsv]\n", argv[0]);
        return EXIT_FAILURE;
    }
    LangModel model = {0};
    if (langmdl_load(&model, argc == 2 ? argv[1] : "data/responses.tsv") != 0)
        return EXIT_FAILURE;
    
    printf(" ┌────────────────────────────┐\n");
    printf(" |        SHAMAANI v1.0       |\n");
    printf(" | Pimeän pohjalan kielimalli |\n");
    printf(" └────────────────────────────┘\n");
    
    int result = parser_run(&model);
    langmdl_free(&model);
    return result;
}
