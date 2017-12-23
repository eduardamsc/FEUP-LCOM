/**
 *  @author Joao Cardoso (jcard@fe.up.pt) ????
 *  Added by pfs@fe.up.pt
 */

/** Format of a xpm-like (there is a real xpm format) pic:
 * <pre>
 * static char *picture_name[] = {
 * "number_of_x_pixels number_of_y_pixels number_of_colors",
 * "one_char_symbol the_char_symbol_color",
 * ... exactly number_of_colors lines as above
 * "any of the above one_char_symbol, exactly number_of_x_pixels times",
 * ... exactly number_of_y_pixels lines as above
 * };
 *
 * Example:
 *
 * static char *pic1[] = {  // the name os the picture, "pic1" in this case
 * "32 13 4",               // number of pixels in the x and y dimension,
 *                          //   and the number of colors, which follows.
 * ". 0",                   // mapping between symbols and colors; in this
 *                          //   case, the "." will be mapped to color 0
 * "x 2",                   // and the "x" to color 2
 * ...                      // the next symbol/color pairs
 * "................................", // first row, exactly 32 pixels, 
 *                                     // all color 0, BLACK
 * "..............xxx...............", // next row has three GREEN pixels
 *                                     // at the center
 * </pre>
 * Colors names/colors number equivalence are at c:/djgpp/include/conio.h
 *
 * Any real xpm picture can be read, as long as there are only 16
 * colors on it, and the colors are the ones available in conio.h.
 *
 *  To use other/more colors than the basic ones, the graphic card
 *  pallete must be programmed.
 * 
 */

static char *pic1[] = {
  "32 13 4",
  ". 0",
  "x 2",
  "o 14",
  "+ 4",
  "................................",
  "..............xxx...............",
  "............xxxxxxx.............",
  ".........xxxxxx+xxxxxx..........",
  "......xxxxxxx+++++xxxxxxx.......",
  "....xxxxxxx+++++++++xxxxxxx.....",
  "....xxxxxxx+++++++++xxxxxxx.....",
  "......xxxxxxx+++++xxxxxxx.......",
  ".........xxxxxx+xxxxxx..........",
  "..........ooxxxxxxxoo...........",
  ".......ooo...........ooo........",
  ".....ooo...............ooo......",
  "...ooo...................ooo...."
};


static char *pic2[] = {
  "32 15 3",
  ". 0",
  "x 4",
  "+ 1", 
  "................................",
  "..............xxx...............",
  "............xxxxxxx.............",
  ".........xxxxxx+xxxxxx..........",
  "......xxxxxxx+++++xxxxxxx.......",
  "......xxxxxxx+++++xxxxxxx.......",
  "++++xxxxxxx+++++++++xxxxxxx+++++",
  "++++xxxxxxx+++++++++xxxxxxx+++++",
  "++++xxxxxxx+++++++++xxxxxxx+++++",
  "......xxxxxxx+++++xxxxxxx.......",
  "......xxxxxxx+++++xxxxxxx.......",
  ".........xxxxxx+xxxxxx..........",
  "............xxxxxxx.............",
  "..............xxx...............",
  "................................",
};

static char *cross[] = {
  "32 32 2",
  ". 0",
  "+ 13", 
  "...............+................",
  "...............+................",
  "...............+................",
  "...............+................",
  "...............+................",
  "...............+................",
  "...............+................",
  "...............+................",
  "...............+................",
  "...............+................",
  "...............+................",
  "...............+................",
  "...............+................",
  ".............++.++..............",
  "............+.....+.............",
  "...........+.......+............",
  "+++++++++++.........++++++++++++",
  "...........+.......+............",
  "............+.....+.............",
  ".............++.++..............",
  "...............+................",
  "...............+................",
  "...............+................",
  "...............+................",
  "...............+................",
  "...............+................",
  "...............+................",
  "...............+................",
  "...............+................",
  "...............+................",
  "...............+................",
  "...............+................", 
};


static char *pic3[] = {
  "64 32 3",
  ". 0",
  "+ 1",
  "o 2",
  "..............................++++++............................",
  ".........................++++++++++++++++.......................",
  "......................++++++++++++++++++++++....................",
  "....................++++++++++++++++++++++++++..................",
  ".................++++++++++++++++++++++++++++++++...............",
  "...............++++++++++++++++++++++++++++++++++++.............",
  ".............++++++++++++++++++++++++++++++++++++++++...........",
  "............++++++++++++++++++++++++++++++++++++++++++..........",
  "..........++++++++++++++++++++++++++++++++++++++++++++++........",
  ".........++++++++++++++++++++++++++++++++++++++++++++++++.......",
  "........+++++++++++++++++++++++++++++++++++++++++++++++++.......",
  ".......+++++++++++++++++++++++++++++++++++++++++++++++++++......",
  "......+++++++++++++++++++++++++++++++++++++++++++++++++++++.....",
  ".....++++++++++++++++++++++++++++++++++++++++++++++++++++++.....",
  "....++++++++++++++++++++++++++++++++++++++++++++++++++++++++....",
  "...++++++++++++++++++++++++++++++++++++++++++++++++++++++++++...",
  "...++++++++++++++++++++++++++++++++++++++++++++++++++++++++++...",
  "..++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++..",
  "..++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++..",
  ".++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++.",
  ".+++++ooo+++++++++ooo+++++++++ooo+++++++++ooo+++++++++ooo++++++.",
  ".++++ooooo+++++++ooooo+++++++ooooo+++++++ooooo+++++++ooooo+++++.",
  ".+++ooooooo+++++ooooooo+++++ooooooo+++++ooooooo+++++ooooooo++++.",
  ".++++ooooo+++++++ooooo+++++++ooooo+++++++ooooo+++++++ooooo+++++.",
  ".+++++ooo+++++++++ooo+++++++++ooo+++++++++ooo+++++++++ooo++++++.",
  ".++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++.",
  ".++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++.",
  ".++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++.",
  "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++",
  "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++",
  "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++",
  "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++",
};

/* this one was converted from a 32 bits colors png! */
 
static char * penguin[] = {
"64 64 9",
"  0",
". 15",
"+ 8",
"@ 14",
"# 4",
"$ 6",
"% 7",
"& 9",
"* 1",
"                                                                ",
"                                                                ",
"                           +#++#&+#+                            ",
"                        +..%......&.+++                         ",
"                     #...+..+.+.+.#.+.+++                       ",
"                    .*#%..+.%.&.#*+.#%.++++                     ",
"                  #.+.#*#%..+.#*#.%#*#+.++++                    ",
"                 .&.%......#*#.%.+.#&.+%#%++++                  ",
"                #*#..........+.+.+.+.+.++#+++++                 ",
"               &#.+..........%.+.&#*#&#*.+&+++++                ",
"              +.+.............#*#.+.......#++++++               ",
"             #+.+..............+.+..........+++++               ",
"            +&.+%..............%+............+++++              ",
"            ++.+#.............................+++++             ",
"           ++.+%.............#..#..#..........+++++             ",
"          +++.++%..........#    .    #........++++++            ",
"          ++++.+...........#   .     #.....%..+++++++           ",
"          ++%+#+&..........#   .     #........+++++++           ",
"         +++#*+#..%.........#  .    #........%+++++++           ",
"         ++++%+++..%..%......#.#..#...%..%#..+++++++++          ",
"        +++++#+&+#.......%..................++++++++++          ",
"        ++++*++.+++.%.#...@@@@@@@@@@@@...%.+++++++++++          ",
"        +++++#+++%#$#.&.+.%@@@@@@@@@@...#++++++++++++++         ",
"        +++++&+.++++*.#.....@@@@@@@@......+++++++++&+++         ",
"       ++++++++++%#%....%....@@@@@@........+++++++++++%         ",
"       +++++++++++#...........@@@@@.........+++++++++++         ",
"       ++++++++++..............@@@...........+++++++++++        ",
"      +++++++++++...............@.............+++++%+++++       ",
"    ++++++++++++.....................%.........+++++++++++      ",
"   ++++++++++++%.........................%..%..%++++%++++++     ",
"  +++++++++++++...............................#.++++++++++++    ",
" +++++++++++++%..%.........................#.....++++++++++++   ",
"++++++++++++++........................%..%..%.+.%++%++++++++++  ",
"++++++++++++++...................................#++%++++++++++ ",
"    +++++++++...%..%.....................%.+.%#.%.++++++        ",
"       ++++++%....................%...........$.#.#+#%+         ",
"       +++++#........................%..#..+.#.%$%+&+++         ",
"        ++++&.#.%..%....................%...*#.#...+#++         ",
"        ++++..%...........................%..%.+.+.+&++         ",
"        +++++...%.....%.........%...%..+..#.#..#.%.+#+          ",
"        ++++..#...%......%...............&.*.+.*#$#&++          ",
"         +++..*#...%.........%.....#..%...#.%.#%.+..++          ",
"         +++.#...#..............%...%..#..&.#*#.+.+++           ",
"          ++.%.&...%..%..%..........#..*#..+..+.#%.++           ",
"          ++..#..+..........%...%..$..%..+..&#*.+.+++           ",
"           +*.+.....%............#..$.#...+.#.+.+%.+            ",
"           +...%.+.#..%.%..#.%.#..%.#.*#%..&.+.+.+++            ",
"            +.+.......#..%.....%...%.#..&#$#%.+.+.+             ",
"            #..%#*#%....#..+..$..+..+.&#.+.+.+%+#&              ",
"             +.....#*#$.+...%.#...+....+.%#*#.+.+               ",
"              +.+.&..%..%.+.#.&.+..&#+.+.+.+.++++               ",
"               %..+.#.+.+.+.*#..%#.$.+.&#*#&#%.+                ",
"                +.%#*#%#$.%#.+.+.+.+%#$#+%#*+++                 ",
"                 @#.#$#%++.+.%.+.&.#*#%+#$##$#                  ",
"          .@.@@@@@@@@@@$#.++.+.#.+.+%#$......@@@@@@@            ",
"        @@@@@.@.@@@@@@@@@$.+$#&+.+.+@@..@....@.@@@$@@$          ",
"       @@@@@@.@@.@$@$@$$@$@+%#$#&#+@@.@..@@.@.@@$@@$$$$+        ",
"      @@@@@@@@@@@@@$@$@$@$$$+%+.+%$$@@$@@.@.@$@$@$$@$$$$        ",
"       @@@@$@$@$@$$@$$$$$$$#++++++#$@$@$$$$@$$@$$$$$$$$+        ",
"        @$@@$@$@$@$$$$$$$$          $$$$@$$$$$$$$$$$$$+         ",
"          $$@$$$$$$$$$$$               $$$$$$$$$$$$+            ",
"                                                                ",
"                                                                ",
"                                                                "};

static char * car[] = {
"100 27 7",
"  0",
"s 1",
"N 2",
"m 3",
"y 4",
"d 5",
"h 6",
"                                                                                                    ",
"                                                        NhydN                                       ",
"              NNNNmmmmmmmmmmNNNNNNNNNN                  Nhssm           NNmmmmmmmmmmNNN             ",
"     NNNmddhyyyysssssssssssssssssyyyyyyyyyyyyyyyyyyyyyyyyyssyyyyyyyyyyyyysssssssssssssyyhmN         ",
"  mhyyysssssssssssssssssssssssyhhhhhhhhhhhhhhhhhhhhhhhhhhhhhsssssssssssssssssssssssssssssssymN      ",
" NyssssssssssssssssysyssssshdmmmmmmmNmmmmmmmmmmmmddddddhhyysssyyhhhhyyysssssssssssssssssssssymNN    ",
" dssssssssssssssssshdhhsssyhhhddddddddddddhhhhhyyyysssssyhdddmmmmmmmmmdysssssssssyyyyyyssssssdmmN   ",
"NyssssssssssssyyyhhddddysssssssssssssssssssssssssssssssymmmmmmmmmmmmmmmmhssssssssshyyhhhsssssshmmN  ",
"NsssssssssshdmmmmmmmmmmmysssssssssssssssssssssssssssssshmmmmmmmmmmmmmmmmmhssssssssyyyyhyhssssssyyhN ",
"mssssssssydmmmmmmmmmmmmmysssssssssssssssssssssssssssssshmmmmmmmmmmmmmmmmmmysssssssssssssysssssssysd ",
"mssssssssdmmmmmmmmmmmmmmysssssssssssssssssssssssssssssshmmmmmmmmmmmmmmmmmmdsssssssssssssssssssssysyN",
"mssssssshmmmmmmmmmmmmmmmysssssssssssssssssssssssssssssshmmmmmmmmmmmmmmmmmmmssssssssssssssssssssssssm",
"dsssssssdmmmmmmmmmmmmmmmysssssssssssssssssssssssssssssshmmmmmmmmmmmmmmmmmmmysssssssssssssssssssssssm",
"dsssssssmmmmmmmmmmmmmmmmysssssssssssssssssssssssssssssshmmmmmmmmmmmmmmmmmmmysssssssssssssssssssssssd",
"dsssssssdmmmmmmmmmmmmmmmysssssssssssssssssssssssssssssshmmmmmmmmmmmmmmmmmmmysssssssssssssssssssssssd",
"mssssssshmmmmmmmmmmmmmmmysssssssssssssssssssssssssssssshmmmmmmmmmmmmmmmmmmmssssssssssssssssssssssssm",
"mssssssssmmmmmmmmmmmmmmmysssssssssssssssssssssssssssssshmmmmmmmmmmmmmmmmmmdsssssssssssssssssssssyssN",
"mssssssssymmmmmmmmmmmmmmysssssssssssssssssssssssssssssshmmmmmmmmmmmmmmmmmmhsssssssssssssssssssssyshN",
"NsssssssssydmmmmmmmmmmmmysssssssssssssssssssssssssssssshmmmmmmmmmmmmmmmmmdsssssssssyyyyshssssssssyN ",
"Nysssssssssssyyhhhhddddhssssssssssssssssssssssssssssssshmmmmmmmmmmmmmmmmdssssssssshyyhhhyssssshdmm  ",
"NdssssssssssssssssshdhhsssyyyhhhhhhhhhhhhhhhyyyyysssssssyddmmmmmmmmmmmdhsssssssssyyyyyyssssssdmmN   ",
" msssssssssssssssssyyyysssydmmmmmmmmNmmmmmmmmmmdddddddhhyyssyyyhhdddhyysssssssssysssssssssssymmN    ",
"  myyssssssssssssssssssssssssyhhhdddddddhhhhhhhhhhhhhhhddhhhsssssssssssssssssssssssssssssssydN      ",
"    NNmmdhhyyysssssssssssssssssssssssyyyyyyyyyyyyyyyyyyyyysssyyyyyyyyyyysssssssssssssssyydmN        ",
"            NNNNmmmddddddddmmmmmmmNNNNNNNNNNNNNNNNNNNNNNNdssdNNNNNNNNNNNNmmdddddddddmmNN            ",
"                                                        NhydN                                       ",
"                                                                                                    "};