/******************************************************************************/
/* Filename:      dispdatahandling.c                                          */
/* Author:        Berger Jonas                                                */
/* Version:       V01                                                         */
/* Description:   Contains printMQVal(), printMeanValCons(),                  */
/*                         printValToTXTFile(), printMeanValToSVGFile(),      */
/*                         printValToSVGFile(), printAxisToSVG(),             */
/*                         printBarsToSVG()                                   */
/******************************************************************************/

/******************************* INCLUDES *************************************/
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

#include <main.h>
#include <dispdatahandling.h>
#include <barcalc.h>

/**************************  PRIVATE PROTOTYPES *******************************/
static int printAxisToSVG(FILE ***fp, int flags[]);
static int printBarsToSVG(FILE ***fp, int humiarray[], int presarray [], int temparray[]);

/******************************************************************************/
/*            S U B R O U T I N E :     printMQVal()                          */
/*----------------------------------------------------------------------------*/
/* Purpose:      Prints given sensor-value to console                         */
/* Input(s):     (long) - Type of measurement                                 */
/*               (int)  - Actual sensor-value                                 */
/* Return value: (int)  - Returns 0 on success and a negative number on       */
/*                        failure                                             */
/******************************************************************************/
int printMQVal(int flags[], long senstype, int sensvalue)
{
  int ret = 0, retpf = 0;
  float presatm = 0.0, tempcels = 0.0, tempfahr = 0.0;
  
  switch(senstype)
  {
    case HumidityVal:
      retpf = printf("Humidity-Value received: %d %%\n", sensvalue);
      if(retpf < 0)
      {
        ret = -1;
      }
      break;
    case PressureVal:
      if(flags[2] == 1) // atm
      {
        presatm = (float)sensvalue / 101325; // conversion PA to atm
        retpf = printf("Pressure-Value received: %.3f atm\n", presatm);
        if(retpf < 0)
        {
          ret = -2;
        }
      }
      else
      {
        retpf = printf("Pressure-Value received: %d PA\n", sensvalue);
        if(retpf < 0)
        {
          ret = -3;
        }
      }
      break;
    case TemperatureVal:
      if(flags[0] == 1) // °C
      {
        tempcels = (float)sensvalue - 273.15; // conversion K to °C
        retpf = printf("Temperature-Value received: %.2f °C\n", tempcels);
        if(retpf < 0)
        {
          ret = -4;
        }
      }
      else
      {
        if(flags[1] == 1) // °F
        {
          tempfahr = ((float)sensvalue - 273.15) * (9/5) + 32; // conversion K to °F
          retpf = printf("Temperature-Value received: %.2f °F\n", tempfahr);
          if(retpf < 0)
          {
            ret = -5;
          }
        }
        else // Kelvin
        {
          retpf = printf("Temperature-Value received: %d K\n", sensvalue);
          if(retpf < 0)
          {
            ret = -6;
          }
        }
      }
      break;
    default:
      ret = -7;
  }
  
  return ret;
}

/******************************************************************************/
/*            S U B R O U T I N E :     printMeanValCons()                    */
/*----------------------------------------------------------------------------*/
/* Purpose:      Prints mean values to console                                */
/* Input(s):     (int *)   - Pointer to array with flags                      */
/*               (float *) - Pointer to array with mean-sensor-values         */
/* Return value: (int)     - Returns 0 on success and a negative number on    */
/*                           failure                                          */
/******************************************************************************/
int printMeanValCons(int flags[], float meanvalues[])
{
  int ret = 0, retpf = 0, retscmp = 0;
  char utemp[5] = "", upres[5] = "", *retscpy = NULL;
  
  // Define Unit corresponding to flags
  if(flags[0] == 1)
  {
    retscpy = strncpy(utemp, "°C", sizeof(utemp));
    retscmp = strncmp(retscpy, "°C", strlen(retscpy));
    if(retscmp != 0)
    {
      ret = -1;
    }
  }
  if(flags[1] == 1)
  {
    retscpy = strncpy(utemp, "°F", sizeof(utemp));
    retscmp = strncmp(retscpy, "°F", strlen(retscpy));
    if(retscmp != 0)
    {
      ret = -2;
    }
  }
  if((flags[0] == 0) && (flags[1] == 0))
  {
    retscpy = strncpy(utemp, "K", sizeof(utemp));
    retscmp = strncmp(retscpy, "K", strlen(retscpy));
    if(retscmp != 0)
    {
      ret = -3;
    }
  }
  if(flags[2] == 1)
  {
    retscpy = strncpy(upres, "atm", sizeof(upres));
    retscmp = strncmp(retscpy, "atm", strlen(retscpy));
    if(retscmp != 0)
    {
      ret = -4;
    }
  }
  else
  {
    retscpy = strncpy(upres, "PA", sizeof(upres));
    retscmp = strncmp(retscpy, "PA", strlen(retscpy));
    if(retscmp != 0)
    {
      ret = -5;
    }
  }
  
  // Print mean-values to console
  retpf = printf("Mean-Values: H: %.1f %% P: %.3f %s T: %.1f %s\n", meanvalues[0], meanvalues[1],
                                                                    upres, meanvalues[2], utemp);
  if(retpf < 0)
  {
    ret = -6;
  }
  
  return ret;
}

/******************************************************************************/
/*            S U B R O U T I N E :     printValToTXTFile()                   */
/*----------------------------------------------------------------------------*/
/* Purpose:      Prints sensor-value to given TXT-file                        */
/* Input(s):     (FILE **) - Pointer to pointer to file which shall be written*/
/*               (int *)   - Pointer to array with flags                      */
/*               (long)    - Type of measurement                              */
/*               (int)     - Actual sensor-value                              */
/* Return value: (int)     - Returns 0 on success and a negative number on    */
/*                           failure                                          */
/******************************************************************************/
int printValToTXTFile(FILE **fp, int flags[], long senstype, int sensvalue)
{
  int ret = 0, retfp = 0, retff = 0;
  float temperg = 0.0, preserg = 0.0;
  
  switch(senstype)
  {
    case HumidityVal:
      retfp = fprintf(*fp, "Humidity-value: %d %%\n", sensvalue);
      if(retfp < 0)
      {
        ret = -1;
      }
      break;
    case PressureVal:
      if(flags[2] == 1)
      {
        preserg = (float)sensvalue / 101325;
        retfp = fprintf(*fp, "Pressure-value: %.3f atm\n", preserg);
      }
      else
      {
        preserg = (float)sensvalue;
        retfp = fprintf(*fp, "Pressure-value: %.0f K\n", preserg);
      }
      if(retfp < 0)
      {
        ret = -2;
      }
      break;
    case TemperatureVal:
      temperg = (float)sensvalue;
      if(flags[0] == 1)
      {
        temperg = (float)sensvalue - 273.15;
        retfp = fprintf(*fp, "Temperature-value: %.1f °C\n", temperg);
      }
      if(flags[1] == 1)
      {
        temperg = ((float)sensvalue - 273.15) * (9/5) + 32;
        retfp = fprintf(*fp, "Temperature-value: %.1f °F\n", temperg);
      }
      if(retfp < 0)
      {
        ret = -3;
      }
      break;
    default:
      ret = -4;
  }
  
  retff = fflush(*fp); // write content to file immediately
  if(retff != 0)
  {
    ret = -5;
  }
  
  return ret;
}

/******************************************************************************/
/*            S U B R O U T I N E :     printMeanValToSVGFile()               */
/*----------------------------------------------------------------------------*/
/* Purpose:      Prints mean-sensor-values to SVG-file                        */
/* Input(s):     (FILE **) - Pointer to pointer to file which shall be written*/
/*               (int *)   - Pointer to array with flags                      */
/*               (float)   - Mean-humidity-value                              */
/*               (float)   - Mean-pressure-value                              */
/*               (float)   - Mean-temperature-value                           */
/* Return value: (int)     - Returns 0 on success and a negative number on    */
/*                           failure                                          */
/******************************************************************************/
int printMeanValToSVGFile(FILE **fp, int flags[], float mhumi, float mpres, float mtemp)
{
  int ret = 0, retfp = 0, retff = 0;
  float tempwidth = 0, preswidth = 0, humiwidth = 0;
  
  // print svg-header
  retfp = fprintf(*fp, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
                       "<svg xmlns=\"http://www.w3.org/2000/svg\"\n"
                       "  version=\"1.1\" baseProfile=\"full\"\n"
                       "  width=\"460px\" height=\"100px\" viewBox=\"0 0 460 100\">\n"
                       "  <title>Barchart of the Mean-Sensor-Values</title>\n"
                       "  <desc>Chart shows the Mean-value of temperature,\n"
                       "        pressure and humidity\n  </desc>\n"); // print svg-header
  if(retfp < 0)
  {
    ret = -1;
  }
  
  /* Temperature */
  if(flags[0] == 1) // print Grad Celsius
  {
    // tempwidth = (210 * (80 - ((39.85 - (mtemp)))) / 100) + 5;
    tempwidth = 210 * (mtemp - (-40.15)) / (39.85 - (-40.15)) + 5;
    if(tempwidth < 5)
    {
      tempwidth = 5;
    }
    retfp = fprintf(*fp, "  <rect fill=\"#FF0000\" width=\"%.0f\" height=\"19\"></rect>\n"
                         "  <text font-family=\"Verdana\" font-size=\"16\" fill=\"#FF0000\" x=\"%.0f\" y=\"9.5\" dy=\".35em\">Mean-Temperature: %.1f °C</text>\n", tempwidth, tempwidth + 5, mtemp);
    if(retfp < 0)
    {
      ret = -2;
    }
  }
  else
  {
    if(flags[1] == 1) // print Grad Fahrenheit
    {
      tempwidth = 210 * (mtemp - (-41.27)) / (103.73 - (-41.27)) + 5;
      if(tempwidth < 5)
      {
        tempwidth = 5;
      }
      retfp = fprintf(*fp, "  <rect fill=\"#FF0000\" width=\"%.0f\" height=\"19\"></rect>\n"
                           "  <text font-family=\"Verdana\" font-size=\"16\" fill=\"#FF0000\" x=\"%.0f\" y=\"9.5\" dy=\".35em\">Mean-Temperature: %.1f °F</text>\n", tempwidth, tempwidth + 5, mtemp);
      if(retfp < 0)
      {
        ret = -3;
      }
    }
    else // print Kelvin
    {
      tempwidth = 210 * (mtemp - MIN_TEMP_K) / (MAX_TEMP_K - (MIN_TEMP_K)) + 5;
      if(tempwidth < 5)
      {
        tempwidth = 5;
      }
      retfp = fprintf(*fp, "  <rect fill=\"#FF0000\" width=\"%.0f\" height=\"19\"></rect>\n"
                           "  <text font-family=\"Verdana\" font-size=\"16\" fill=\"#FF0000\" x=\"%.0f\" y=\"9.5\" dy=\".35em\">Mean-Temperature: %.1f K</text>\n", tempwidth, tempwidth + 5, mtemp);
      if(retfp < 0)
      {
        ret = -4;
      }
    }
  }
  
  /* Pressure */
  if(flags[2] == 1) // print atm
  {
    preswidth = 210 * (mpres - (0.947)) / (1.381 - (0.947)) + 5;
    if(preswidth < 5)
    {
      preswidth = 5;
    }
    retfp = fprintf(*fp, "  <rect fill=\"#51565C\" width=\"%.0f\" height=\"19\" y=\"20\"></rect>\n"
                         "  <text font-family=\"Verdana\" font-size=\"16\" fill=\"#51565C\" x=\"%.0f\" y=\"28\" dy=\".35em\">Mean-Pressure: %.3f atm</text>\n", preswidth, preswidth + 5, mpres);
    if(retfp < 0)
    {
      ret = -5;
    }
  }
  else // print Pascal
  {
    preswidth = 210 * (mpres - MIN_PRES_PA) / (MAX_PRES_PA - MIN_PRES_PA) + 5;
    if(preswidth < 5)
    {
      preswidth = 5;
    }
    retfp = fprintf(*fp, "  <rect fill=\"#51565C\" width=\"%.0f\" height=\"19\" y=\"20\"></rect>\n"
                         "  <text font-family=\"Verdana\" font-size=\"16\" fill=\"#51565C\" x=\"%.0f\" y=\"28\" dy=\".35em\">Mean-Pressure: %.1f PA</text>\n", preswidth, preswidth + 5, mpres);
    if(retfp < 0)
    {
      ret = -6;
    }
  }
  
  /* Humidity */
  humiwidth = 210 * (mhumi - MIN_HUMI_REL) / (MAX_HUMI_REL - MIN_HUMI_REL) + 5;
  if(humiwidth < 5)
  {
    humiwidth = 5;
  }
  retfp = fprintf(*fp, "  <rect fill=\"#0C71E0\" width=\"%.0f\" height=\"19\" y=\"40\"></rect>\n"
                       "  <text font-family=\"Verdana\" font-size=\"16\" fill=\"#0C71E0\" x=\"%.0f\" y=\"48\" dy=\".35em\">Mean-Humidity: %.1f %%</text>\n", humiwidth, humiwidth + 5, mhumi);
  if(retfp < 0)
  {
    ret = -7;
  }
  
  /* Axis */
  retfp = fprintf(*fp, "  <line x1=\"5\" y1=\"68\" x2=\"215\" y2=\"68\" stroke=\"black\" stroke-width=\"2px\"/>\n"
                       "  <text font-family=\"Verdana\" font-size=\"12\" x=\"10\" y=\"88\" dy=\".35em\">MIN</text>\n"
                       "  <text font-family=\"Verdana\" font-size=\"12\" x=\"185\" y=\"88\" dy=\".35em\">MAX</text>\n");
  if(retfp < 0)
  {
    ret = -8;
  }
  
  retfp = fprintf(*fp, "</svg>\n"); // print svg-ending
  if(retfp < 0)
  {
    ret = -9;
  }
  
  retff = fflush(*fp); // write content to file immediately
  if(retff != 0)
  {
    ret = -10;
  }
  
  return ret;
}

/******************************************************************************/
/*            S U B R O U T I N E :     printValToSVGFile()                   */
/*----------------------------------------------------------------------------*/
/* Purpose:      Prints sensor-values to SVG-file                             */
/* Input(s):     (FILE **) - Pointer to pointer to file which shall be written*/
/*               (int *)   - Pointer to array with flags                      */
/*               (int *)   - Pointer to array with humidity-values            */
/*               (int *)   - Pointer to array with pressure-values            */
/*               (int *)   - Pointer to array with temperature-values         */
/* Return value: (int)     - Returns 0 on success and a negative number on    */
/*                           failure                                          */
/******************************************************************************/
int printValToSVGFile(FILE **fp, int flags[], int humiarray[], int presarray[], int temparray[])
{
  int ret = 0, retfp = 0, retff = 0, retpa = 0, retpb = 0;
  
  // print svg-header
  retfp = fprintf(*fp, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
                       "<svg xmlns=\"http://www.w3.org/2000/svg\"\n"
                       "  version=\"1.1\" baseProfile=\"full\"\n"
                       "  width=\"450px\" height=\"650px\" viewBox=\"0 0 450 650\">\n"
                       "  <title>Barchart of the Sensor-Values</title>\n"
                       "  <desc>Chart shows the last 10 sensore-values of temperature,\n"
                       "        pressure and humidity\n  </desc>\n"); // print svg-header
  if(retfp < 0)
  {
    ret = -1;
  }
  
  retpa = printAxisToSVG(&fp, flags); // print all axis, units and headings
  if(retpa < 0)
  {
    ret = -2;
  }
  
  retpb = printBarsToSVG(&fp, humiarray, presarray, temparray); // print values
  if(retpb < 0)
  {
    ret = -3;
  }
  
  retfp = fprintf(*fp, "</svg>\n"); // print svg-ending
  if(retfp < 0)
  {
    ret = -4;
  }
  
  retff = fflush(*fp); // write content to file immediately
  if(retff != 0)
  {
    ret = -5;
  }
  
  return ret;
}

/******************************************************************************/
/*            S U B R O U T I N E :     printAxisToSVG()                      */
/*----------------------------------------------------------------------------*/
/* Purpose:      Prints all axis and headings to SVG-file                     */
/* Input(s):     (FILE ***) - Pointer to pointer to pointer to file which     */
/*                            shall be written                                */
/*               (int *)    - Pointer to array with flags                     */
/* Return value: (int)      - Returns 0 on success and a negative number on   */
/*                            failure                                         */
/******************************************************************************/
static int printAxisToSVG(FILE ***fp, int flags[])
{
  int ret = 0, retfp = 0;
  
  char *Kmax = "<text font-family=\"Verdana\" font-size=\"12\" x=\"5\" y=\"55\" dy=\".35em\">313 K</text>";
  char *Kmin = "<text font-family=\"Verdana\" font-size=\"12\" x=\"5\" y=\"195\" dy=\".35em\">233 K</text>";
  char *Cmax = "<text font-family=\"Verdana\" font-size=\"12\" x=\"5\" y=\"55\" dy=\".35em\">39.85 °C</text>";
  char *Cmin = "<text font-family=\"Verdana\" font-size=\"12\" x=\"5\" y=\"195\" dy=\".35em\">-40.15 °C</text>";
  char *Fmax = "<text font-family=\"Verdana\" font-size=\"12\" x=\"5\" y=\"55\" dy=\".35em\">103.73 °F</text>";
  char *Fmin = "<text font-family=\"Verdana\" font-size=\"12\" x=\"5\" y=\"195\" dy=\".35em\">-40.27 °F</text>";
  
  char *PAmax = "<text font-family=\"Verdana\" font-size=\"12\" x=\"5\" y=\"255\" dy=\".35em\">140000 PA</text>";
  char *PAmin = "<text font-family=\"Verdana\" font-size=\"12\" x=\"5\" y=\"395\" dy=\".35em\">96000 PA</text>";
  char *ATMmax = "<text font-family=\"Verdana\" font-size=\"12\" x=\"5\" y=\"255\" dy=\".35em\">1.382 atm</text>";
  char *ATMmin = "<text font-family=\"Verdana\" font-size=\"12\" x=\"5\" y=\"395\" dy=\".35em\">0.947 atm</text>";
  
  /******* Create Temperature Graph Axis *******/
  retfp = fprintf(**fp, "  <text font-family=\"Verdana\" font-size=\"15\" fill=\"#FF0000\" x=\"100\" y=\"35\" dy=\".35em\">Temperature:</text>\n"
                        "  <line x1=\"100\" y1=\"200\" x2=\"100\" y2=\"50\" stroke=\"black\" stroke-width=\"5px\"/>\n");
  if(retfp < 0)
  {
    ret = -1;
  }
  
  if((flags[0] == 0) && (flags[1] == 0)) // Kelvin axis
  {
    retfp = fprintf(**fp, "  %s\n"
                          "  %s\n", Kmax, Kmin);
    if(retfp < 0)
    {
      ret = -2;
    }
  }
  if(flags[0] == 1) // grad celsius axis
  {
    retfp = fprintf(**fp, "  %s\n"
                          "  %s\n", Cmax, Cmin);
    if(retfp < 0)
    {
      ret = -3;
    }
  }
  if(flags[1] == 1) // grad fahrenheit axis
  {
    retfp = fprintf(**fp, "  %s\n"
                          "  %s\n", Fmax, Fmin);
    if(retfp < 0)
    {
      ret = -4;
    }
  }
  
  /******* Create Pressure Graph Axis *******/
  retfp = fprintf(**fp, "  <text font-family=\"Verdana\" font-size=\"15\" fill=\"#51565C\" x=\"100\" y=\"235\" dy=\".35em\">Pressure:</text>\n"
                        "  <line x1=\"100\" y1=\"400\" x2=\"100\" y2=\"250\" stroke=\"black\" stroke-width=\"5px\"/>\n");
  if(retfp < 0)
  {
    ret = -5;
  }
  
  if(flags[2] == 1) // atm axis
  {
    retfp = fprintf(**fp, "  %s\n"
                          "  %s\n", ATMmax, ATMmin);
    if(retfp < 0)
    {
      ret = -6;
    }
  }
  else // PA axis
  {
    retfp = fprintf(**fp, "  %s\n"
                          "  %s\n", PAmax, PAmin);
    if(retfp < 0)
    {
      ret = -7;
    }
  }
  
  /******* Create Humidity Graph Axis *******/
  retfp = fprintf(**fp, "  <text font-family=\"Verdana\" font-size=\"15\" fill=\"#0C71E0\" x=\"100\" y=\"435\" dy=\".35em\">Humidity:</text>\n"
                        "  <line x1=\"100\" y1=\"600\" x2=\"100\" y2=\"450\" stroke=\"black\" stroke-width=\"5px\"/>\n"
                        "  <text font-family=\"Verdana\" font-size=\"12\" x=\"5\" y=\"455\" dy=\".35em\">100 %%</text>\n"
                        "  <text font-family=\"Verdana\" font-size=\"12\" x=\"5\" y=\"595\" dy=\".35em\">10 %%</text>\n");
  if(retfp < 0)
  {
    ret = -8;
  }
  
  return ret;
}

/******************************************************************************/
/*            S U B R O U T I N E :     printBarsToSVG()                      */
/*----------------------------------------------------------------------------*/
/* Purpose:      Prints sensor-values as Bars to SVG-file                     */
/* Input(s):     (FILE **) - Pointer to pointer to file which shall be written*/
/*               (int *)   - Pointer to array with humidity-values            */
/*               (int *)   - Pointer to array with pressure-values            */
/*               (int *)   - Pointer to array with temperature-values         */
/* Return value: (int)     - Returns 0 on success and a negative number on    */
/*                           failure                                          */
/******************************************************************************/
static int printBarsToSVG(FILE ***fp, int humiarray[], int presarray [], int temparray[])
{
  int ret = 0, i = 0, retfp = 0;
  int tempx1 = 115, tempx2 = 115, presx1 = 115, presx2 = 115, humix1 = 115, humix2 = 115;
  int tempy1 = 200, tempy2 = 195, presy1 = 400, presy2 = 395, humiy1 = 600, humiy2 = 595;
  
  // temperature
  for(i = 0; i < NUM_OF_MEASUREMENTS; i++)
  {
    tempy2 = calcBarTemp(temparray[i]);
    retfp = fprintf(**fp, "  <line x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" stroke=\"red\" stroke-width=\"15px\"/>\n",
                             tempx1, tempy1, tempx2, tempy2);
    if(retfp < 0)
    {
      ret = -1-i;
    }
    
    tempx1 = tempx1 + 30;
    tempx2 = tempx2 + 30;
  }
  
  // pressure
  for(i = 0; i < NUM_OF_MEASUREMENTS; i++)
  {
    presy2 = calcBarPres(presarray[i]);
    retfp = fprintf(**fp, "  <line x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" stroke=\"grey\" stroke-width=\"15px\"/>\n",
                             presx1, presy1, presx2, presy2);
    if(retfp < 0)
    {
      ret = -1-NUM_OF_MEASUREMENTS-i;
    }
    
    presx1 = presx1 + 30;
    presx2 = presx2 + 30;
  }
  
  // humidity
  for(i = 0; i < NUM_OF_MEASUREMENTS; i++)
  {
    humiy2 = calcBarHumi(humiarray[i]);
    retfp = fprintf(**fp, "  <line x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" stroke=\"blue\" stroke-width=\"15px\"/>\n",
                             humix1, humiy1, humix2, humiy2);
    if(retfp < 0)
    {
      ret = -1-(2*NUM_OF_MEASUREMENTS)-i;
    }
    
    humix1 = humix1 + 30;
    humix2 = humix2 + 30;
  }
  
  return ret;
}
