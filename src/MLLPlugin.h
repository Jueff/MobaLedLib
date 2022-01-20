#ifndef __MLLPLUGIN__
#define __MLLPLUGIN__

/*
 MobaLedLib: LED library for model railways
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

 Copyright (C) 2018 - 2021  Hardi Stengelin: MobaLedLib@gmx.de
 
 this file: Copyright (C) 2021 Jürgen Winkler: MobaLedLib@a1.net

 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.

 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA
 -------------------------------------------------------------------------------------------------------------
 
 
 Revision History :
~~~~~~~~~~~~~~~~~
17.11.21:  Versions 1.0 (Jürgen)

*/

#ifndef DEBUG_MLL_PLUGINS
  //#define DEBUG_MLL_PLUGINS 0x0f
#endif
#ifdef DEBUG_MLL_PLUGINS
  const char* DebugName = "PluginProcessor";
#endif  


class MLLPlugin
{
  
	public:
    virtual void setup() = 0;
    // The main loop
		virtual void loop() = 0;
#if defined(ESP32) || defined(ARDUINO_RASPBERRY_PI_PICO)
    // On multicore CPUs the additional loop. FastLED and MobaLedLib is always processed in main (=other) loop
		virtual void loop2() = 0;
#endif    
};

class PluginProcessor
{
  uint8_t  count = 0;
  MLLPlugin** plugins;

  public:
  PluginProcessor(MLLPlugin* plugins[], uint8_t count)
  {
#if (DEBUG_MLL_PLUGINS&0x01)==0x01
    { char s[80]; sprintf(s, "%s created with %d plugins", DebugName, count); Serial.println(s); Serial.flush();} // Debug
#endif      
    this->count = count;
    this->plugins = plugins;
  }
  
  void setup()
  {
#if (DEBUG_MLL_PLUGINS&0x01)==0x01
    { char s[80]; sprintf(s, "%s calls setup for %d plugins", DebugName, count); Serial.println(s); Serial.flush();} // Debug
#endif      
    for (int i=0;i<count; i++)
    {
      plugins[i]->setup();
    }
#if (DEBUG_MLL_PLUGINS&0x01)==0x01
    { char s[80]; sprintf(s, "%s setup done", DebugName); Serial.println(s); Serial.flush();} // Debug
#endif      
  }
  
  void loop()
  {
#if (DEBUG_MLL_PLUGINS&0x02)==0x02
    { char s[80]; sprintf(s, "%s calls loop for %d plugins", DebugName, count); Serial.println(s); Serial.flush();} // Debug
#endif      
    for (int i=0;i<count; i++)
    {
#if (DEBUG_MLL_PLUGINS&0x04)==0x04
    { char s[80]; sprintf(s, "%s calls loop for plugin %d", DebugName, i); Serial.println(s); Serial.flush();} // Debug
#endif      
      plugins[i]->loop();
    } 
  }

#if defined(ESP32) || defined(ARDUINO_RASPBERRY_PI_PICO)
  void loop2()
  {
#if (DEBUG_MLL_PLUGINS&0x02)==0x02
    { char s[80]; sprintf(s, "PluginProcessor calls loop2 for %d plugins", count); Serial.println(s); Serial.flush();} // Debug
#endif      
    for (int i=0;i<count; i++)
    {
#if (DEBUG_MLL_PLUGINS&0x04)==0x04
    { char s[80]; sprintf(s, "PluginProcessor calls loop2 for plugin %d", i); Serial.println(s); Serial.flush();} // Debug
#endif      
      plugins[i]->loop2();
    } 
  }
#endif
  
};
#endif