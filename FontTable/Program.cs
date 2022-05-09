using System;
using System.Collections.Generic;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace FontTable
{
    internal class Program
    {

        public class FontTable
        {
            string character;
            Font font;
            Bitmap bmp;
            Graphics g;
            int width;
            int height;
            public FontTable(string toConvert, string fontName, int fontSize)
            {
                character = toConvert;
                font = new Font(fontName, fontSize, GraphicsUnit.Pixel);
                bmp = new Bitmap(1, 1);
                g = Graphics.FromImage(bmp);
                width = (int)g.MeasureString(character, font).Width;
                height = (int)g.MeasureString(character, font).Height;
                bmp = new Bitmap(width, height);
                g = Graphics.FromImage(bmp);
                g.Clear(Color.White);
                g.TextRenderingHint = System.Drawing.Text.TextRenderingHint.SingleBitPerPixel;
            }
            
            public void testDraw()
            {
                Console.WriteLine($"initiating draw of character {character} in font {font.Name}...");
                g.DrawString(character, font, new SolidBrush(Color.Black), 0, 0);

                for (int i = 0; i < width; i++)
                {
                    for (int j = 0; j < height; j++)
                    {
                        Color toPrint = bmp.GetPixel(i, j);
                        Console.Write(toPrint.Name);
                    }
                    Console.Write("\n");
                }
                Console.WriteLine("...finished drawing character");
                Console.WriteLine("-----------------");

                g.Flush();
                g.Dispose();
            }
        }

        static void Main(string[] args)
        {
            FontTable ft = new FontTable("H", "Arial", 12);
            ft.testDraw();

            FontTable ft2 = new FontTable("H", "Times New Roman", 12);
            ft2.testDraw();


            Console.ReadLine();
        }
    }
}
