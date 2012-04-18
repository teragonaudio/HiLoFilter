About HiLoFilter
================

HiLoFilter is a simple hipass and lopass filter which can be easily controlled
with a single knob. It is loosely inspired by the same type of filter found on
some Pioneer DJM mixers, and also the [The Pilgrim]
(http://arcticanaudio.com/effects/thepilgrim.html), another great plugin which
provides roughly the same functionality.

So for those who haven't used such a filter, here's how it works:

* When the knob is in the center position, no audio processing is performed
* Turning the knob to the right from center will start to cut low frequencies
* Turning the knob to the lift will cut the high frequencies

Features
--------

* Configurable "dead zone" determines how big the middle position of the knob
  should be. Without a dead zone, filtering will be performed unless the knob
  is at the exact center. Many users have controllers without sticky points in
  the center, and during a performance that "close enough" factor ensures that
  the cleanest possible sound is given without accidentally cutting a bit from
  the high/low end.
* Configurable, independent high/low limits for the filter. Sometimes you want
  the filter to cut 100% of the sound at each end, sometimes not. HiLoFilter
  has got you covered.
* Each filter's range can be independently configured from 20Hz-20kHz
* Logarithmic scaling for frequency parameters. This ensures that filtering on
  both the high and low ends of the frequency spectrum is accurate and sane.
* Filter resonance. Enough said.
* Great CPU performance, uses nearly 50% as much power as The Pilgrim (more on
  that below).
* Dead zone goes up to 11. Rock on!

Performance
-----------

Part of the reason I wrote HiLoFilter was that The Pilgrim's performance
was starting to impact my DJ setup. I was constructing a loop mixing set with
Ableton Live, where I had 2 "decks", each with 8 independent channels. To save
knobs, I wanted one filter per channel, so I started looking for a nice
DJM-like filter. I found The Pilgrim and was quite happy with it, but lacking
a dead zone, I built an audio effect rack with a utility plugin (passthru) and
two instances of The Pilgrim. The result was a liveset with 2 * 8 * 2 = 32
instances.

A single instance of The Pilgrim uses nearly no CPU, but 32 of them takes a
noticeable chunk. I started experiencing audio dropouts from this and all the
other stuff in my liveset, and got rid of the effect chain, which reduced the
liveset to 16 instances. It was better, but still too high.

HiLoFilter isn't anything special. I coded it in an afternoon, and the filter
code is from the long-retired Convolver plugin from [Teragon Audio]
(http://www.teragonaudio.com). But written in highly optimized C++, the
performance is almost twice as good as The Pilgrim.

I benchmarked HiLoFilter against The Pilgrim in an empty liveset with 100
instances (more details on the test setup below). The resulting CPU usage
reported by Ableton Live was roughly 31% for 100 instances of HiLoFilter, and
58% for 100 instances of The Pilgrim, a CPU savings of nearly 50%.

As HiLoFilter has a passthru dead zone, I benchmarked HiLoFilter both in the
passthru state and when filtering and found that the difference was
negligible.

The Verdict
-----------

The Pilgrim is a great plugin, and I can definitely recommend that users try
both it and HiLoFilter to determine which one they prefer more, based on the
sound and features. For my needs, a dead zone and low CPU usage were enough to
necessitate writing this plugin, but in a situation where one wants only one
such plugin on the master channel controlled by a sticky knob, these might not
matter.

Limitations
-----------

Currently HiLoFilter is only available as a 32-bit Universal Binary for Mac
OSX. It should run as both a VST2 and AudioUnit plugin. The plugin should also
build under Windows as a 32-bit VST2 plugin (and there are project files ready
to do just that), but I'm too lazy too install VS2010 and test under this
platform. If someone out there is so willing, grab the source on GitHub and
send me a built DLL, which I would be happy to post on my website.

Bugs and Feedback
-----------------

HiLoFilter was, as mentioned, coded in a short amount of time. It has not been
heavily tested, though the filter code was recycled from a more well-tested
plugin. You might very well find bugs, so beware! If you do find any, please
contact me at support (a) teragonaudio (dot) com. Likewise, if you're into
GitHub, you can file a bug report there.

Licensing
---------

HiLoFilter is licensed under the GPL. It uses the excellent [Juce framework]
(http://rawmaterialsoftware.com/juce], and therefore is also GPL'd.

Testing Configuration
---------------------

Both plugins were tested in an empty liveset, processing an unwarped MP3
playing through 20 audio effect chain containing 5 instances each. My computer
is a MacBook Pro 2x2.66 Core 2 Duo with 8Gb RAM and an Intel 310 SSD (3Gbps),
running OSX 10.6.8. The test was performed with Ableton Live 8.3 with a buffer
size of 512. Your results may vary.

