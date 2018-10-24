/*
 * SSD1306 I2C Driver Test
 * Mikukonai 2017
 *
 */
#include "SSD1306.h"

unsigned char cache_bmp[128 * 32 / 8] = 
{
	//page0, seg0-7
	// 0xFF, 0xFF, 0xFF, 0xFF,	0xFF, 0xFF, 0xFF, 0xFF,
	//page0, seg8-15
	// 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,

	// 0xff, 0xff, 0xff, 0xff,
	// 0xff, 0xff, 0xff, 0xff,
	// 0xff, 0xff, 0xff, 0xff,
	// 0xff, 0xff, 0xff, 0xff,

	// 0xdd, 0xdd, 0xdd, 0xdd,
	// 0xdd, 0xdd, 0xdd, 0xdd,
	// 0xdd, 0xdd, 0xdd, 0xdd,
	// 0xdd, 0xdd, 0xdd, 0xdd,
	// 0xff, 0xff, 0xff, 0xff,
	// 0xff, 0xff, 0xff, 0xff,
	// 0xff, 0xff, 0xff, 0xff,
	// 0xff, 0xff, 0xff, 0xff,
};

static void cache_setXY(unsigned char row, unsigned char col, unsigned char onff)
{
	unsigned int index;
	unsigned char page, seg, value;

	page = row % SSD1306_Max_Y / 8; //PAGE ROW = 8;
	seg = col % SSD1306_Max_X;
	index = page * SSD1306_Max_X + seg;
	value = cache_bmp[index];

	if(onff) {
		value |= (1 << (row % 8));
	} else {
		value &= ~(1 << (row % 8));
	}
	cache_bmp[index] = value;
}

static void cache_flush(unsigned int depth)
{
	if(depth) {
		depth = depth <    sizeof(cache_bmp)?:sizeof(cache_bmp);
		SSD1306_drawBitmap(cache_bmp, depth);
	} else {
		SSD1306_drawBitmap(cache_bmp, sizeof(cache_bmp));
	}
}

static void cache_drawPoint(unsigned char row, unsigned char col)
{
	return cache_setXY(row, col, 1);
}

int main()
{
	int x, y, v;

	SSD1306_init();
	SSD1306_clearDisplay();
	SSD1306_setBrightness(255);
	SSD1306_setPageMode();

	v = 1;
	for(y=0; y<SSD1306_Max_Y; y++) {
		for(x=0; x<SSD1306_Max_X; x++) {
			// v = 1 - v;
			if(y)
				cache_setXY(y-1, x-1, 1 - v);
			cache_setXY(y, x, v);
		}
		cache_flush(0);
		// cache_flush(y * 128 / 8 + x);
	}

	// cache_drawPoint(16, 64);

	cache_flush(0);

	// SSD1306_putString("ABCDEFGHIJKLMNOP");
	// SSD1306_setTextXY(1,0);
	// SSD1306_putString("QRSTUVWXYZabcdef");
	// SSD1306_setTextXY(2,0);
	// SSD1306_putString("192.168.250.250");
	// SSD1306_setTextXY(3,0);
	// SSD1306_putString("wxyz0123456789!\"");
	// SSD1306_setTextXY(4,0);
	// SSD1306_putString("#$%^&*(),.<>[]{}");

	// SSD1306_activateScroll();
	close(ssd1306_i2c_devfd);
	return 0;
}
