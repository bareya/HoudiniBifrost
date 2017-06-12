/*
MIT License

Copyright (c) 2017 Piotr Barejko

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "GEO/GEO_IOTranslator.h"

class GEO_BifrostIOTranslator : public GEO_IOTranslator
{
public:
	GEO_BifrostIOTranslator();
	GEO_BifrostIOTranslator(const GEO_BifrostIOTranslator& other);
	virtual ~GEO_BifrostIOTranslator();

	virtual GEO_IOTranslator* duplicate() const;
	virtual const char*	formatName() const;
	virtual int	checkExtension(const char *name);

	virtual int	checkMagicNumber(unsigned magic);

	virtual GA_Detail::IOStatus fileLoad(GEO_Detail *gdp, UT_IStream &is, bool ate_magic);
	virtual GA_Detail::IOStatus fileSave(const GEO_Detail *gdp, std::ostream &os);
};

