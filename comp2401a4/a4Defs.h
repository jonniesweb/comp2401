#define MAX_STR       32
#define MAX_BYTES      4

/*  Movie list related data types  */

typedef enum {C_COMEDY, C_DRAMA, C_ACTION, C_HORROR,
              C_SF, C_ADVENTURE, C_WESTERN} GenreType;



/*  Movie list related functions  */
void mainMenu(int*);
void getMovieData(MovieNodeType**);
void deleteMovie(MovieNodeType**, char*);
void addToMovieList(MovieNodeType**, MovieType*);
void printMovieData(MovieNodeType*);
void printMoviesByGenre(MovieNodeType*, char*);
char *getGenre(GenreType);
void cleanupMovies(MovieNodeType*);
void cleanupMovieNodes(MovieNodeType*);

/*  Utility functions  */
void dumpList(MovieNodeType*);
void dumpBytes(char*, int);
void convertToBytes(int num, unsigned char *bytes);

/*  Bit related functions  */
int  getIntBit(int, int);
unsigned char setBit(unsigned char, int);
unsigned char clearBit(unsigned char, int);
