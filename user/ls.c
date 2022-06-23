#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"

int dflag = 0;




char*
fmtname(char *path)
{
  static char buf[DIRSIZ+1];
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  // Return blank-padded name.
  if(strlen(p) >= DIRSIZ)
    return p;
  memmove(buf, p, strlen(p));
  memset(buf+strlen(p), ' ', DIRSIZ-strlen(p));
  return buf;
}

void
ls(char *path)
{
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;
  struct stat target_st;

  if((fd = open(path, O_NO_LINK)) < 0){
    fprintf(2, "ls: cannot open %s\n", path);
    return;
  }

  if(fstat(fd, &st) < 0){
    fprintf(2, "ls: cannot stat %s\n", path);
    close(fd);
    return;
  }

  switch(st.type){
//---------------------------------------------------------------------------
    case T_SYMLINK:
    if(dflag){
      printf("print on debug %d", dflag);
    }
    readlink(path, buf, 512);
    stat(buf, &target_st);
    printf("%s -> %s %d %d 0\n", fmtname(path), buf, target_st.type,st.ino);
    break;
//---------------------------------------------------------------------------
  case T_FILE:
    if(dflag){
      printf("print on debug %d", dflag);
    }
    printf("%s %d %d %l\n", fmtname(path), st.type, st.ino, st.size);
    break;

  case T_DIR:
    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
          if(dflag){
      printf("print on debug %d", dflag);
    }
      printf("ls: path too long\n");
      break;
    }
    strcpy(buf, path);
    p = buf+strlen(buf);
    *p++ = '/';
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
      if(de.inum == 0)
        continue;
      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;
      if(stat(buf, &st) < 0){
        printf("ls: cannot stat %s\n", buf);
        continue;
      }
//---------------------------------------------------------------------------
      if(st.type == T_SYMLINK){
      if(dflag){
      printf("print on debug %d", dflag);
      }
        char t [256];
        readlink(buf, t, 256);
        stat(t, &target_st);
        printf("%s -> %s %d %d 0\n", fmtname(buf),t, target_st.type, st.ino);

      }
//---------------------------------------------------------------------------

      else{
      printf("%s %d %d %d\n", fmtname(buf), st.type, st.ino, st.size);
      }
    }
    break;
  }
  close(fd);
}

int
main(int argc, char *argv[])
{
  int i;

  if(argc < 2){
    ls(".");
    exit(0);
  }
  for(i=1; i<argc; i++)
    ls(argv[i]);
  exit(0);
}
