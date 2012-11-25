#ifndef CHORD_H
#define CHORD_H

#include "Tuning.h"
#include "Tonality.h"


class Chord
{
	public:
		Chord();

	private:
		Tuning m_tuning;
		Tonality m_tonality;
};

#endif // CHORD_H
