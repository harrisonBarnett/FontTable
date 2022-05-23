using System;
using System.Collections.Generic;
using System.Drawing;
using System.Text;

namespace FontTable
{
    // TODO:
    // bitshift left char hex value to produce 7-bit representation for "7-bit mode"
    //  

    internal class Program
    {
        public class CharArr
        {
            //  char array components
            public string character;
            private Font font;
            private Bitmap bmp;
            private Graphics g;
            private int width;
            private int height;

            public CharArr(string toConvert, string fontName, int fontSize)
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
            public void drawChar()
            {
                g.DrawString(character, font, new SolidBrush(Color.Black), 0, 0);

                for (int i = 0; i < height; i++)
                {
                    for (int j = 0; j < width; j++)
                    {
                        Color toPrint = bmp.GetPixel(j, i);
                        Console.Write(toPrint.Name);
                    }
                    Console.Write("\n");
                }

                g.Flush();
                g.Dispose();
            }
        }

        public class FontTable
        {
            // array of characters
            public List<CharArr> table = new List<CharArr>();

            //  font table components
            public string fontName;
            public int fontSize;
            public string bitMode;
            public FontTable(string name, int size, string mode)
            {
                fontName = name;
                fontSize = size;
                bitMode = mode;
            }

            public void generateTable()
            {
                Console.WriteLine($"Generating table of type {fontName}, size {fontSize}");
                List<CharArr> tmpTable = new List<CharArr>();

                if(bitMode == "A")
                    // 7-bit mode, only produce 128 ASCII chars
                {
                    for (var i = 32; i < 128; i++)
                    {
                        string toConvert = Convert.ToChar(i).ToString();
                        CharArr toAdd = new CharArr(toConvert, fontName, fontSize);
                        tmpTable.Add(toAdd);
                    }
                } else
                {
                    // 8-bit mode, produce 256 ASCII chars
                    for (var i = 32; i < 256; i++)
                    {
                        string toConvert = Convert.ToChar(i).ToString();
                        CharArr toAdd = new CharArr(toConvert, fontName, fontSize);
                        tmpTable.Add(toAdd);
                    }
                }
                table.Add(new CharArr(" ", fontName, fontSize));
                table = tmpTable;
            }
            
            public void printTable()
            {
                table.ForEach(character =>
                {
                    character.drawChar();
                });
            }
            
        }

        static void Main(string[] args)
        {
            Console.WriteLine("Generating tables...");

            FontTable xL = new FontTable("Arial", 24, "A");
            xL.generateTable();
            FontTable L = new FontTable("Arial", 15, "A");
            L.generateTable();
            FontTable M = new FontTable("Arial", 12, null);
            M.generateTable();
            FontTable S = new FontTable("Arial", 9, null);
            S.generateTable();

            Console.WriteLine("Finished.");

            xL.printTable();

            Console.ReadLine();
        }
    }
}
