/* 
 * File:   sound.h
 * Author: Bruno
 *
 * Created on 28 juillet 2014, 16:56
 */

#ifndef SOUND_H
#define	SOUND_H

#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#endif	/* SOUND_H */

void SoundContinuous(char freq, char duration, char volume);

void SoundIntermittent(char freq, char soundduration, char SilentDuration, char cycles, char volume);

void SoundStop(void);