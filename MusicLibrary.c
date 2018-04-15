#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STRMAX 100
#define SONGMAX 1024
char *infile = "myMusic";
char *outfile = "myMusic";
int quit=0;

struct Song{
        char title[STRMAX];
        char artist[STRMAX];
        int year_published;
};

struct Song music_library[SONGMAX];
int current_number_of_songs=0;

void write_song(int index,FILE *f,struct Song s){
        fprintf(f,"%d\n%s\n%s\n%d\n",index+1,s.title,s.artist,s.year_published);
        return;
}


char read_command(){
        printf("myMusic Command: ");
        char command = getchar();
        while ((getchar()) != '\n');
        return command;
}


void crunch_up_from_index(int index){
        for(int i=index;i<current_number_of_songs;i++){
                music_library[i]=music_library[i+1];
        }
        current_number_of_songs--;
        return;
}

void crunch_down_from_index(int index){
        for(int i=current_number_of_songs-1;i>=index;i--){
                        music_library[i+1]=music_library[i];
                }
        current_number_of_songs++;
        return;
}

void add_song_to_MusicLibrary(struct Song s){
        if(current_number_of_songs==0){
                music_library[0]=s;
                current_number_of_songs++;
        }
        else{
                int index;
                for(int i=0; i<current_number_of_songs; i++){
                        if(strcmp(s.title,music_library[i].title)<=0){
                                index=i;
                                break;
                        }
                        index=current_number_of_songs;

                }
                crunch_down_from_index(index);
                music_library[index]=s;
        }
        return;
}

FILE * open_file(char *file,char *r_w){
        FILE *f;
        f=fopen(file,r_w);
        if (f==NULL){
                printf("ERROR\n");
        }
        return f;
}

void load_MusicLibrary(char *infile){
        FILE *inf;
        inf=open_file(infile,"r");
        char line[STRMAX];
        int i=1,index=-1,select;
        struct Song s;
        while(fgets(line,STRMAX,inf)!=NULL){
                strtok(line,"\n");
                select=i%4;
                switch(select){
                        case 1:
                                index++;
                                break;
                        case 2:
                                strcpy(s.title,line);
                                break;
                        case 3:
                                strcpy(s.artist,line);
                                break;
                        case 0:
                                s.year_published=atoi(line);
                                add_song_to_MusicLibrary(s);
                                break;
                }
                i++;

        }
        fclose(inf);
        return;

}

void store_MusicLibrary(char *outfile){
        FILE *ouf;
        ouf=open_file(outfile,"w");
        for(int i=0; i<current_number_of_songs; i++){
                write_song(i,ouf,music_library[i]);
        }
        fclose(ouf);
        return;
}

void print_MusicLibrary(){
        for(int i=0; i<current_number_of_songs; i++){
                printf("(%s, %s, %d)\n",music_library[i].title,music_library[i].artist,music_library[i].year_published);
        }
        return;
}


int find_index_of_song_with_name(char *name){
        int begin=0;
        int end=current_number_of_songs-1;
        int index=begin+(end-begin)/2;
        if(strcmp(name,music_library[begin].title)==0){
                return begin;
        }
        if(strcmp(name,music_library[end].title)==0){
                return end;
        }
        while(index>begin && index<end){
                if(strcmp(name,music_library[index].title)==0){
                        return index;
                }
                else if(strcmp(name,music_library[index].title)<0){
                        end=index;
                        index=begin+(end-begin)/2;
                }
                else{
                        begin=index;
                        index=begin+(end-begin)/2;
                }
        }
        return -1;
}

void remove_song_from_MusicLibrary_by_name(char *name){
        int index=find_index_of_song_with_name(name);
        if(index==-1){
                printf("Song Not In Library\n");
                return;
        }
        crunch_up_from_index(index);
        return;
}


void evaluate_command(char command){
        char line[STRMAX];
        struct Song s;
        int index;
        switch(command){
                case 'I':
                        printf("Enter Song Title: ");
                        fgets(s.title,STRMAX,stdin);
                        strtok(s.title,"\n");
                        printf("Enter Song Artist: ");
                        fgets(s.artist,STRMAX,stdin);
                        strtok(s.artist,"\n");
                        printf("Enter Year Published: ");
                        s.year_published=atoi(fgets(line,STRMAX,stdin));
                        add_song_to_MusicLibrary(s);
                        break;
                case 'P':
                        printf("Music Library:\n");
                        print_MusicLibrary();
                        break;
                case 'D':
                        printf("Enter Song Title: ");
                        fgets(line,STRMAX,stdin);
                        strtok(line,"\n");
                        remove_song_from_MusicLibrary_by_name(line);
                        break;
                case 'L':
                        printf("Enter Song Title: ");
                        fgets(line,STRMAX,stdin);
                        strtok(line,"\n");
                        index=find_index_of_song_with_name(line);
                        if(index==-1){
                                printf("Song Not In Library\n");
                        }
                        else{
                                printf("(%s, %s, %d)\n",music_library[index].title,music_library[index].artist,music_library[index].year_published);
                        }
                        break;
                case 'Q':
                        quit=1;
                        store_MusicLibrary(outfile);
                        break;
                default:
                        printf("Invalid Entry\n");
        }
        return;
}
int main(){
        char command;
        load_MusicLibrary(infile);
        while(!quit){
                command = read_command();
                evaluate_command(command);
        }
        return 0;
}
