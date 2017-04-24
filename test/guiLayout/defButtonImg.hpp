Image getDefButtonImg()
{
	Image img(vec2(128,128));
	
	img.forEach([](vec2s p,Color &c,Image &img) {
		vec2s s = img.getSize();
		
		if (p.x == 0 || p.y == 0 || p.x+1 == s.w || p.y+1 == s.h)
		{
			if ((p.x == 0) + (p.y == 0) + (p.x+1 == s.w) + (p.y+1 == s.h) >= 2)
			{
				c = Color(0,0,0,0);
			}
			else 
				c = Color(32,32,32,150);
		}
		else if (p.x == 1 || p.y == 1 || p.x+2 == s.w || p.y+2 == s.h)
		{
			if ((p.x == 1) + (p.y == 1) + (p.x+2 == s.w) + (p.y+2 == s.h) >= 2)
			{
				c = Color(32,32,32);
			}
			else 
				c = Color(230,100);
		}
		else
		{
			c = Color(8,8,8,64);
			
			float yval = float(p.y) / s.h;
			c += Color( std::sqrt(1 - yval) * 210,0);				
		}
	});
	
	return img;
}