#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <libxml/xmlschemastypes.h>


/* **********************************************************************************************
 *                          Les prototypes de fonctions :                                       *
 * **********************************************************************************************/
void print_usage();                             // USAGE DE LA COMMANDE
int validateXML(char *filenamexml);             // valider xml
int validateJSON(FILE *filename);               // valider json
void xmlTrace(char *xml);                       // tracer xml
/* ***********************************************************************************************/

int main(int argc, char *argv[]) {

    if (argc<2) {
        printf("Veuillez verifiez la syntaxe d'utilisation de la commande XJ_Convertor\n");
        print_usage();
    }
    FILE *jsonFile;
    int optch;
    extern int opterr;
    char format[] = "tf:h:i:o:";

    /* Variables pour compter le nombre de parametres renseignes par l'utilisateur */
    int iflag = 0 ,tflag = 0 ,hflag = 0 ,oflag = 0, fflag = 0 ;

    /* Pour le format saisi l'utilisateur (ie: xml = 1 -> format xml ou json =1 -> format json)*/
    int xml = 0, json = 0 ;

    opterr = 1;
    char *fichierInput; // Variable pour stocker le nom du fichier en entree
    char *fichierOuput; // Variable pour stocker le nom du fichier en sortie
    char *formatFich ;  // Variable pour stocker le format du fichier saisi

    while ((optch = getopt(argc, argv, format)) != -1)
        switch (optch) {
         case 'i':
                iflag = 1 ;
                formatFich = optarg;
                break;

            case 't':
                tflag = 1 ;
                break;
            case 'h':
                hflag = 1 ;
                fichierInput = optarg;
                break;
            case 'f':
                fflag = 1;
                fichierInput = optarg;
                break;
            case 'o' :
                oflag = 1 ;
                fichierOuput = optarg;
                break;
            case '?' :
                if (optopt=='i' || optopt=='h' || optopt=='f' || optopt=='o') {

                    fprintf(stderr,"Option -%c attend un argument.\n",optopt);
                    printf("Exemple d’utilisation : XJ_Convertor -i xml -f monfichier.xml -o monfichier.svg\n");
                    exit(0);
                }else if(isprint(optopt)) {
                    fprintf(stderr,"Option inconnu -%c .\n",optopt);
                    exit(0);
                }else {
                    fprintf(stderr,"Option inconnu %c .\n",optopt);
                    print_usage();
                }
                return 1;
            default:
                print_usage();
                exit(EXIT_FAILURE);
        }

    /* Parcourir toute  la commande pour signaler s'il y a une erreur au niveau des options ou arguments */
    for (; optind < argc; ++optind)
        printf ("Pas d'option ni d\'argument : %s\n",argv[optind]);
        //exit(0);

     if (iflag == 1 ) {

         if (strcmp("xml",formatFich) == 0 || strcmp("json",formatFich) == 0) {

             if (strcmp("xml",formatFich) == 0 ) {         // Si le format choisi est le format xml
                 xml = 1 , json = 0;                       // On met la variable xml a 1 et json a 0
             }else if (strcmp("json",formatFich) == 0 ) {  // Si le format choisi est le format json
                 json = 1, xml = 0;                        // On met la variable json a 1
             }
             if (fflag == 1 || hflag == 1) {           // On verifie si l'une des options -f ou -h est presente
                 if (oflag == 1) {                         // On verifie si l'option -o est presente
                     if (fflag == 1) {
                         if (xml == 1 ) {
                             validateXML(fichierInput);
                             if (tflag == 1) {
                                 xmlTrace(fichierInput);
                             }

                         } else if(json == 1) {
                             jsonFile = fopen(fichierInput,"rt");
                             validateJSON(jsonFile);
                             fclose(jsonFile);
                         }
                     }
                     printf("Generation du fichier svg ...\n");
                 }else {
                     printf("Veuillez specifier l'option -o pour la generation du fichier de sortie svg \n");
                 }
             }else {
                 printf("Veuillez specifier l'option -f pour un <fichier> ou l'option -h pour un <flux http> \n");
                 print_usage();
             }


         }
         // le format choisi n'est ni xml ni json
         else {
             printf("L'option -i a pour argument <xml> ou <json> \n");
             print_usage();
         }
     }else {
         print_usage();
     }

    return 0;

}


void print_usage() {
    printf("UTILISATION : XJ_Convertor [-i xml/json] [-t ][-h url_FluxHTTP] [-f FichierInput] -o nomFichier.svg\n"
           "\t[-i xml/json] : permet de dire si l’input est en xml ou en json\n"
           "\t[-t] : permet de dire si on veut les traces (affichage sur écran de la liste des entités et des relations)\n"
           "\t[-h url_FluxHTTP] : permet de désigner un input en flux http\n"
           "\t[-f FichierInput] : permet de désigner un input de type fichier\n"
           "\t-o nomFichier.svg : permet de designer le fichier de sortie\n"

           "\nExemple d’utilisation : XJ_Convertor -i xml -f monfichier.xml -o monfichier.svg\n");
}



