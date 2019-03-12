#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <json-c/json.h>
#include <libconfig.h>
#include <curl/curl.h>
#include <string.h>
#include <syslog.h>
//#include "headers.h"//all libraries decalared in headers.h file.
void splitstrings(char *str,json_object *);//this function splitstrings cpuinfo and meminfo data.
void splitwords(char *str,json_object *);//this function splitwords for loadavg file.
void passsplitwords(char *,json_object *);//this function splitwords for uptime file data.
void uptimedata(char *,json_object *);//this function get the uptime data and spliting into words into uptimedata.
void converttime(char *,json_object *);//this function convert uptime data into time format.
void convertvalue(char *,json_object *);//this function passing to uptime time data into data function. 
void passkeyandvalue(char *,char *,json_object *);//this function after splitingstrings that key and value pass to the this function. 
void data(json_object *);//this function entire data all functions parsing to json-c data and that data send to server at a time
void fun(void)
//int main(void)
{
printf("function start");
    json_object * jobj = json_object_new_object();


    FILE *fcpu,*fmem,*fload,*fuptime,*fptr;//file pointer declarations.
   //variable declarations.
      char str[100];
      const  char *str1,*str2,*str3[4];
      long int res=0;
      int p=0,i,j=0,count=0,n=0;
      char ptr[10][10]={"cpuinfo","meminfo","loadavg","uptime"};//2d array declaratioons with deefault strings.
     


      

        if((fcpu=fopen("/proc/cpuinfo","r"))==NULL)//this function check the file open read mode or not.
        {  
           printf("file not be existing\n");
           exit(1);
         }
        if((fmem=fopen("/proc/meminfo","r"))==NULL)//this function check the file open read mode or not.

        {
           printf("file not be existing\n");
           exit(1);
        }
        if((fload=fopen("/proc/loadavg","r"))==NULL)//this function check the file open read mode or not.

        {
           printf("file not be existing\n");
           exit(1);
        }
        if((fuptime=fopen("/proc/uptime","r"))==NULL)//this function check the file open read mode or not.

        {
            printf("file not be existing\n");
            exit(1);
        }


   config_t cfg;//reepresent cnfiguration.

   config_setting_t *metrics;//represent configuration seeettings.

   config_init(&cfg);//represent configure installed or not.

       fptr=fopen("/etc/data/metricsdata.conf","r");//open existing file for specific path.
       if(fptr==NULL)
       {
            printf("there is no config file\n");
            openlog("config file information",LOG_CONS | LOG_NDELAY,LOG_LOCAL0);
            syslog(LOG_ERR,"there is no config file in /etc folder --:%m");
            closelog();
            exit(1);
       }
  
       fseek(fptr, 0L, SEEK_END); //move the cursor starting point to end point.
       res = ftell(fptr);//size of the specific file. 
       rewind(fptr);//rewind the cursor position.
       if(!res)
       { 
              printf("your giving empty config file \n");
              openlog("config file information",LOG_CONS | LOG_NDELAY,LOG_LOCAL0);
              syslog(LOG_ERR,"In config file there is no data give a empty file --:%m");
              closelog();
              return ;
       }
       if(!config_read(&cfg,fptr))//read connfigure file data
       {
             printf("in config file put the data in right format\n");
             openlog("config file information",LOG_CONS | LOG_NDELAY,LOG_LOCAL0);
             syslog(LOG_ERR,"In config file write proper data like config file format --:%m");
             closelog();
             exit(1);
       }
       printf("\n..........................................................................................\n");
       printf("\n.......................config file read successfully...........................................\n");
       openlog("config file information",LOG_CONS | LOG_NDELAY,LOG_LOCAL0);
       syslog(LOG_ERR,"config file data read successfully. --:%m");
       closelog();

       if(config_lookup_string(&cfg,"diagnostic_url",&str1))//this funciton lookup the value in given specific path.
            printf("the diagnostic url is-------------- %s\n",str1);
       else
           {
            openlog("config file information",LOG_CONS | LOG_NDELAY,LOG_LOCAL0);
            syslog(LOG_ERR,"In config file there is no url--:%m");
            closelog();
            printf("the url is not given\n");
            return ;
           }
       if(config_lookup_string(&cfg,"diagnostic_host",&str2))//this function lookup the value in given specific paht.
            printf("the diagnostic host is------------------ %s\n",str2);
       else
          { 
            openlog("config file information",LOG_CONS | LOG_NDELAY,LOG_LOCAL0);
            syslog(LOG_ERR,"In config file there is diagnostic_host config --:%m");
            closelog();
            return ;
          }

            printf("the diagnostic host  is not given\n");
       if( config_lookup_int(&cfg,"port",&p))//this  function lookup the value in given specific path.
            printf("the port number is-------------- %d\n",p);
       else
       {  
           printf("the port is not given\n");
           openlog("config file information",LOG_CONS | LOG_NDELAY,LOG_LOCAL0);
           syslog(LOG_ERR,"In config file there is no port number --:%m");
           closelog();
           return ;
       }
       metrics = config_lookup(&cfg, "data.metrics");//this function locate the setting in the cnfiguraion cnfig specified path.
      if(metrics!=NULL)
      {

      count = config_setting_length(metrics);//this function returns the number of strings in group.
      printf("I have %d params:\n", count);
      for (n = 0; n < count; n++) {
            str3[n]=config_setting_get_string_elem(metrics,n);//These functions set the value at the specified index index in the setting setting to value
          printf("%s\n",str3[n]);

    }
}
      else
      {
           printf("in your config file there is no metrics setings like cpuinfo and meminnfo and loadavg and uptime\n");
           openlog("config file information",LOG_CONS | LOG_NDELAY,LOG_LOCAL0);
           syslog(LOG_ERR,"In config file there is no metrics settings like cpuinfo and meminfo and loadavg and uptime-- :%m");
           closelog();

           return ;
      }  
     if(!count)
     {
         printf("in config file there is no params like cpuinfo and meminfo and loadavg and uptime also send entire data\n");
          openlog("config file information",LOG_CONS | LOG_NDELAY,LOG_LOCAL0);
          syslog(LOG_ERR,"send server default data like cpuinfo and meminfo and loadavg and uptime also --:%m");
            closelog();

         goto move;
     }
     for(i=0;i<=count-1;i++)
     {
           if(strcmp(ptr[i],str3[i])==0)
           { 
              j=i;
           }
    
     switch(j)
     { 
     case 0:
          {
                 while(fgets(str,90,fcpu)!=NULL)//this function read the string in allready open file in read mode.
                 {
                     // printf("%s\n",str);
                     splitstrings(str,jobj);//called to splitsrings funcion and pass too the strinng and json object.
                 } 
         printf("\n...........................................................................................\n");
         printf("\n.................................cpuinfo data................................................\n");
             rewind(fcpu);//rewind the cursor posittion in file.
             data(jobj);//called to data function and pass to the json object.
             break;
         }
   case 1:
         {
              while(fgets(str,90,fcpu)!=NULL)//this function read the string in allready open file in read mode.
              { 
                  // printf("%s\n",str);
                   splitstrings(str,jobj);
              }  
             while(fgets(str,90,fmem)!=NULL)//this function read the string in allready open file in read mode.
             {
                   // printf("%s\n",str);
                    splitstrings(str,jobj);
             }
      printf("\n.............................................................................................\n");
      printf("\n..................................cpuinfo and meminfo data...................................\n");
           rewind(fcpu);
           rewind(fmem);
           data(jobj);
           break;
         }
   case 2:
         {
               while(fgets(str,90,fcpu)!=NULL)//this function read the string in allready open file in read mode.
               { 
                   // printf("%s\n",str);
                   splitstrings(str,jobj);
               }
               while(fgets(str,90,fmem)!=NULL)//this function read the string in allready open file in read mode.
               {
                  // printf("%s\n",str);
                     splitstrings(str,jobj);
               }  
               while(fgets(str,90,fload)!=NULL)//this function read the string in allready open file in read mode.
               {
                   // printf("%s\n",str);
                    splitwords(str,jobj);
               }
        printf("\n............................................................................................\n");
        printf("\n..........................cpuinfo and meminfo and loadavg data...............................\n");
            rewind(fcpu);
            rewind(fmem);
            rewind(fload);
            data(jobj);
            break;
          }
   case 3:
         {
              while(fgets(str,90,fcpu)!=NULL)//this function read the string in allready open file in read mode.
              {
                  // printf("%s\n",str);
                   splitstrings(str,jobj);
              } 
              while(fgets(str,90,fmem)!=NULL)//this function read the string in allready open file in read mode.
              {
                   // printf("%s\n",str);
                     splitstrings(str,jobj);
              }
              while(fgets(str,90,fload)!=NULL)//this function read the string in allready open file in read mode.
              {
                      // printf("%s\n",str);
                         splitwords(str,jobj);
              }
              while(fgets(str,90,fuptime)!=NULL)//this function read the string in allready open file in read mode.
              {
                   // printf("%s\n",str);
                     uptimedata(str,jobj);
               }
     printf("\n...............................................................................................\n");
     printf("\n...................cpuinfo and meminfo and loadavg and uptime data.............................\n");

            rewind(fcpu);
            rewind(fmem);
            rewind(fload);
            rewind(fuptime);
            data(jobj); 
            break; 
          }
   default:
           {
              move:
                 while(fgets(str,90,fcpu)!=NULL)//this function read the string in allready open file in read mode.
                {
                           // printf("%s\n",str);
                             splitstrings(str,jobj);
                }  
                 while(fgets(str,90,fmem)!=NULL)//this function read the string in allready open file in read mode.
                {   
                    // printf("%s\n",str);
                      splitstrings(str,jobj);
                } 
                while(fgets(str,90,fload)!=NULL)//this function read the string in allready open file in read mode.
                {
                        // printf("%s\n",str);
                        splitwords(str,jobj);
                }
               while(fgets(str,90,fuptime)!=NULL)//this function read the string in allready open file in read mode.
               {
                    // printf("%s\n",str);
                     uptimedata(str,jobj);
               }
    printf("\n................................................................................................\n");
    printf("\n...............default data like cpuinfo and meminfo and loadavg and uptime.......................\n");
          rewind(fcpu);
          rewind(fmem);
          rewind(fload);
          rewind(fuptime);
          data(jobj);
       }
    }
 }   
   
}

void splitstrings(char *str,json_object *jobj)
{
  
   /*Creating a json object*/
   //  json_object * jobj = json_object_new_object();


    int i,k;
    char key[80],value[800];


    //  printf("frist file data split into key and value pairs\n");
     if(strcmp(str,"\n"))
     {
      for(i=0;str[i]!=':';i++)
      {
           
           key[i]=str[i];

      }
       key[i]='\0';

      for(i=0;key[i];i++)
      {
      // printf("%c\n",key[i]);
          if(key[i]==' ' || key[i]=='\t')
          {
              key[i]='\0';

          }
      }
     i++;
      for(k=0;str[i]!='\n';i++,k++)
      {
         value[k]=str[i];
      }
      value[k]='\0';
     // printf("key......%s",key);
     // printf("value.......%s\n",value);

      passkeyandvalue(key,value,jobj);
    
  }
}

void splitwords(char *str,json_object *jobj)
{
 char splitStrings[10][10];//can store 10 words of 10 characters
// char prestrings[10][50]={"node_load1","node_load5","node_load15","current_running_process/total_running_process","process_id"};
    int i,j=0,cnt=0;
    for(i=0;i<=(strlen(str));i++)
    {
        // if space or NULL found, assign NULL into splitStrings[cnt]
        if(str[i]==' '||str[i]=='\n')
        {
            splitStrings[cnt][j]='\0';
            cnt++;  //for next word
            j=0;    //for next word, init index to 0
        }
        else
        {
            splitStrings[cnt][j]=str[i];
            j++;
        }
     }
  
  //  printf("\nOriginal String is: %s",str);
   // printf("\nStrings (words) after split by space:\n");
       for(i=0;i < cnt;i++)
       {
       // printf("%s\n",splitStrings[i]);

       // printf("%s\n",prestrings[i]);
       // passkeyandvalue(prestrings[i],splitStrings[i],jobj);
        passsplitwords(splitStrings[i],jobj);
        
        }

}
void passsplitwords(char *value,json_object *jobj)
{
   char prestrings[10][50]={"node_load1","node_load5","node_load15","current_running_process/total_running_process","process_id"};
   static int i=0;
   passkeyandvalue(prestrings[i++],value,jobj);
}
void uptimedata(char *str,json_object *jobj)
{
   char splitStrings[10][10];//can store 10 words of 10 characters

     int i,j=0,cnt=0;
    for(i=0;i<=(strlen(str));i++)
    {
        // if space or NULL found, assign NULL into splitStrings[cnt]
        if(str[i]==' '||str[i]=='\n')
        {
            splitStrings[cnt][j]='\0';
            cnt++;  //for next word
            j=0;    //for next word, init index to 0
        }
        else
        {
            splitStrings[cnt][j]=str[i];
            j++;
        }
     }

    //printf("\nOriginal String is: %s",str);
    //printf("\nStrings (words) after split by space:\n");
       for(i=0;i < cnt;i++)
       
       // printf("%s\n",splitStrings[i]);
        converttime(splitStrings[i],jobj);


}

void converttime(char *str,json_object *jobj)
{
 int  h, m, s,sec;
 char x[20],y[10],z[10];
 sec=atoi(str);
 h = (sec/3600);

  m = (sec -(3600*h))/60;

  s = (sec -(3600*h)-(m*60));
 // printf("%d:%d:%d\n",h,m,s);
  sprintf(x,"%d",h);
  sprintf(y,"%d",m);
  sprintf(z,"%d",s);
  strcat(x,":");
  strcat(x,y);
  strcat(x,":");
  strcat(x,z);
//  printf("%s\n",x);
  convertvalue(x,jobj);

}
void convertvalue(char *value,json_object *jobj)
{
    char prestrings[10][70]={"how_long_the_sytem_has_been_running-H:M:S","how_much_of_that_time_the_machine_has_spen_idle-H:M:S"};
    static int i=0;
    passkeyandvalue(prestrings[i++],value,jobj);
}

void passkeyandvalue(char *key,char *value,json_object *jobj)
{
        /*Creating a json string*/
     json_object *jstring = json_object_new_string(value);


   /*Form the json object*/
     json_object_object_add(jobj,key,jstring);

  /*Now printing the json object*/
   // printf ("The json object created: %sn",json_object_to_json_string(jobj));
}

void data(json_object *jobj)
{
   printf ("The json object created: %s\n",json_object_to_json_string(jobj));
   //curl declarations.
   /*  CURL *curl;
     CURLcode res;
     curl = curl_easy_init();
     char v[]="http://192.168.0.170:8080/op/Measurements";
    
    struct curl_slist *headers = NULL ;

    headers = curl_slist_append(headers, "Content-Type: application/json");


    if(curl)
     {
          curl_easy_setopt(curl, CURLOPT_URL,v);

          //  tell it to "upload" to the URL
         // curl_easy_setopt(curl,CURLOPT_UPLOAD,1L);
          curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
           //  curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
           curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
          //    curl_easy_setopt(curl,CURLOPT_UPLOAD,1L);//enable data upload(it's like large files)


          printf("\n......................................server response...................................\n");         


               curl_easy_setopt(curl, CURLOPT_POSTFIELDS,json_object_to_json_string(jobj));

         // set where to read from (on Windows you need to use READFUNCTION too)
         // printf("upload file data into server\n");
         // curl_easy_setopt(curl, CURLOPT_READDATA,jobj);
         // Perform the request, res will get the return code
          res = curl_easy_perform(curl);
         // Check for errors
          if(res != CURLE_OK)
                 fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));

         // always cleanup
          curl_easy_cleanup(curl);
    }*/

}
