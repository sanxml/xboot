/*
 * sys-copyself.c
 *
 * Copyright(c) 2007-2018 Jianjun Jiang <8192542@qq.com>
 * Official site: http://xboot.org
 * Mobile phone: +86-18665388956
 * QQ: 8192542
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

#include <xboot.h>

extern unsigned char __image_start;
extern unsigned char __image_end;
extern unsigned char __heap_start;
extern void return_to_fel(void);
extern void sys_mmu_init(void);
extern void sys_uart_putc(char c);
extern void sys_decompress(char * src, int slen, char * dst, int dlen);
extern void sys_spi_flash_init(void);
extern void sys_spi_flash_exit(void);
extern void sys_spi_flash_read(int addr, void * buf, int count);

struct zdesc_t {
	uint8_t magic[4];
	uint8_t crc[4];
	uint8_t ssize[4];
	uint8_t dsize[4];
};

enum {
	BOOT_DEVICE_FEL	= 0,
	BOOT_DEVICE_SPI	= 1,
	BOOT_DEVICE_MMC	= 2,
};

static int get_boot_device(void)
{
	u32_t * t = (void *)0x00000058;

	if(t[0] == 0x1)
		return BOOT_DEVICE_FEL;
	return BOOT_DEVICE_SPI;
}

void sys_copyself(void)
{
	struct zdesc_t z;
	uint32_t ssize, dsize;
	int d = get_boot_device();
	void * mem, * tmp;
	u32_t size;

	if(d == BOOT_DEVICE_FEL)
	{
		sys_uart_putc('B');
		sys_uart_putc('o');
		sys_uart_putc('o');
		sys_uart_putc('t');
		sys_uart_putc(' ');
		sys_uart_putc('t');
		sys_uart_putc('o');
		sys_uart_putc(' ');
		sys_uart_putc('F');
		sys_uart_putc('E');
		sys_uart_putc('L');
		sys_uart_putc(' ');
		sys_uart_putc('m');
		sys_uart_putc('o');
		sys_uart_putc('d');
		sys_uart_putc('e');
		sys_uart_putc('\r');
		sys_uart_putc('\n');
		return_to_fel();
	}
	else if(d == BOOT_DEVICE_SPI)
	{
		mem = (void *)&__image_start;
		tmp = (void *)&__heap_start;
		size = &__image_end - &__image_start;
		sys_mmu_init();

		sys_spi_flash_init();
		sys_spi_flash_read(16384, &z, sizeof(struct zdesc_t));
		sys_spi_flash_exit();
		if((z.magic[0] == 'L') && (z.magic[1] == 'Z') && (z.magic[2] == '4') && (z.magic[3] == ' '))
		{
			ssize = (z.ssize[0] << 24) | (z.ssize[1] << 16) | (z.ssize[2] << 8) | (z.ssize[3] << 0);
			dsize = (z.dsize[0] << 24) | (z.dsize[1] << 16) | (z.dsize[2] << 8) | (z.dsize[3] << 0);
			sys_spi_flash_init();
			sys_spi_flash_read(16384 + sizeof(struct zdesc_t), tmp, ssize);
			sys_spi_flash_exit();
			sys_decompress(tmp, ssize, mem, dsize);
		}
		else
		{
			sys_spi_flash_init();
			sys_spi_flash_read(0, mem, size);
			sys_spi_flash_exit();
		}
	}
	else if(d == BOOT_DEVICE_MMC)
	{
		mem = (void *)&__image_start;
		size = (&__image_end - &__image_start + 512) >> 9;
		sys_mmu_init();
	}
}
