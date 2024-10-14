
#include "Audio.h"  // Using ESP32-audioI2S library

#include "FS.h"
//#include "SD.h"     // Only if SD card functions are required in this module
// A library for playing music on the DAC of the ESP32
//
// Not available in library manager, so needs to be manually downloaded
// https://github.com/schreibfaul1/ESP32-audioI2S

// Digital I/O used
#define SD_CS          5
#define SPI_MOSI      23
#define SPI_MISO      19
#define SPI_SCK       18
#define I2S_DOUT      25
#define I2S_BCLK      27
#define I2S_LRC       26

//Audio audio;
Audio audio(true, I2S_DAC_CHANNEL_LEFT_EN);

String MP3List[] = {"http://soundfxcenter.com/animals/birds/8d82b5_Lesser_Whitethroat_Sylvia_Curruca_Sound_Effect.mp3",
  "http://soundfxcenter.com/animals/birds/8d82b5_Bird_Red_Rumped_Parakeet_Sound_Effect.mp3",
  "http://soundfxcenter.com/animals/wild-animals/8d82b5_Wolf_Howling_Sound_FX.mp3",
  "http://soundfxcenter.com/holiday-and-festive/halloween/8d82b5_Ghoul_Laugh_Sound_Effect.mp3",
  "http://soundfxcenter.com/home-and-office/doors-and-windows/8d82b5_Doorbell_Door_Sound_Effect.mp3",
  "https://github.com/schreibfaul1/ESP32-audioI2S/raw/master/additional_info/Testfiles/Olsen-Banden.mp3",
  "https://github.com/schreibfaul1/ESP32-audioI2S/raw/master/additional_info/Testfiles/Pink-Panther.wav"};

void init_AudioESP32() {
    //pinMode(SD_CS, OUTPUT);      
    //digitalWrite(SD_CS, HIGH);
    //SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI);
    //SD.begin(SD_CS);
    audio.forceMono(true);
    //audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
    audio.setVolume(audioVolume); // default 0...21
    //audio.connecttohost("http://soundfxcenter.com/animals/birds/8d82b5_Lesser_Whitethroat_Sylvia_Curruca_Sound_Effect.mp3");
//  or alternative
//  audio.setVolumeSteps(64); // max 255
//  audio.setVolume(63);    
//
//  *** radio streams ***
//  audio.connecttohost("http://stream.antennethueringen.de/live/aac-64/stream.antennethueringen.de/"); // aac
//  audio.connecttohost("http://mcrscast.mcr.iol.pt/cidadefm");                                         // mp3
//  audio.connecttohost("http://www.wdr.de/wdrlive/media/einslive.m3u");                                // m3u
//  audio.connecttohost("https://stream.srg-ssr.ch/rsp/aacp_48.asx");                                   // asx
//  audio.connecttohost("http://tuner.classical102.com/listen.pls");                                    // pls
//  audio.connecttohost("http://stream.radioparadise.com/flac");                                        // flac
//  audio.connecttohost("http://stream.sing-sing-bis.org:8000/singsingFlac");                           // flac (ogg)
//  audio.connecttohost("http://s1.knixx.fm:5347/dein_webradio_vbr.opus");                              // opus (ogg)
//  audio.connecttohost("http://stream2.dancewave.online:8080/dance.ogg");                              // vorbis (ogg)
//  audio.connecttohost("http://26373.live.streamtheworld.com:3690/XHQQ_FMAAC/HLSTS/playlist.m3u8");    // HLS
//  audio.connecttohost("http://eldoradolive02.akamaized.net/hls/live/2043453/eldorado/master.m3u8");   // HLS (ts)
//  *** web files ***
//    audio.connecttohost("https://github.com/schreibfaul1/ESP32-audioI2S/raw/master/additional_info/Testfiles/Pink-Panther.wav");        // wav
//  audio.connecttohost("https://github.com/schreibfaul1/ESP32-audioI2S/raw/master/additional_info/Testfiles/Santiano-Wellerman.flac"); // flac
//  audio.connecttohost("https://github.com/schreibfaul1/ESP32-audioI2S/raw/master/additional_info/Testfiles/Olsen-Banden.mp3");        // mp3
//  audio.connecttohost("https://github.com/schreibfaul1/ESP32-audioI2S/raw/master/additional_info/Testfiles/Miss-Marple.m4a");         // m4a (aac)
//  audio.connecttohost("https://github.com/schreibfaul1/ESP32-audioI2S/raw/master/additional_info/Testfiles/Collide.ogg");             // vorbis
//  audio.connecttohost("https://github.com/schreibfaul1/ESP32-audioI2S/raw/master/additional_info/Testfiles/sample.opus");             // opus
//  *** local files ***
//  audio.connecttoFS(SD, "/test.wav");     // SD
//  audio.connecttoFS(SD_MMC, "/test.wav"); // SD_MMC
//  audio.connecttoFS(SPIFFS, "/test.wav"); // SPIFFS

//  audio.connecttospeech("Wenn die Hunde schlafen, kann der Wolf gut Schafe stehlen.", "de"); // Google TTS
}

bool hanlde_AudioESP32()
{
    audio.loop();
    return audio.isRunning();
}

void playAudio(int number){
  audio.setVolume(audioVolume); // default 0...21
  audio.connecttohost((MP3List[number].c_str()));
}

void playAudio(String url){
  audio.setVolume(audioVolume); // default 0...21
  url.replace("{baseURL}",baseURL);
  audio.connecttohost(url.c_str());
}

// optional
void audio_info(const char *info){
    if (DEBUG_OUTPUT > 3) {Serial.print("info        "); Serial.println(info);}
}
void audio_id3data(const char *info){  //id3 metadata
    if (DEBUG_OUTPUT > 3) {Serial.print("id3data     ");Serial.println(info);}
}
void audio_eof_mp3(const char *info){  //end of file
    if (DEBUG_OUTPUT > 3) {Serial.print("eof_mp3     ");Serial.println(info);}
}
void audio_showstation(const char *info){
    if (DEBUG_OUTPUT > 3) {Serial.print("station     ");Serial.println(info);}
}
void audio_showstreamtitle(const char *info){
    if (DEBUG_OUTPUT > 3) {Serial.print("streamtitle ");Serial.println(info);}
}
void audio_bitrate(const char *info){
    if (DEBUG_OUTPUT > 3) {Serial.print("bitrate     ");Serial.println(info);}
}
void audio_commercial(const char *info){  //duration in sec
    if (DEBUG_OUTPUT > 3) {Serial.print("commercial  ");Serial.println(info);}
}
void audio_icyurl(const char *info){  //homepage
    if (DEBUG_OUTPUT > 3) {Serial.print("icyurl      ");Serial.println(info);}
}
void audio_lasthost(const char *info){  //stream URL played
    if (DEBUG_OUTPUT > 3) {Serial.print("lasthost    ");Serial.println(info);}
}
void audio_eof_speech(const char *info){
    if (DEBUG_OUTPUT > 3) {Serial.print("eof_speech  ");Serial.println(info);}
}