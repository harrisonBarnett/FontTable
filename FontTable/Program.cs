﻿using System;
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
        public class CharArr
        {
            //  char array components
            string character;
            Font font;
            Bitmap bmp;
            Graphics g;
            int width;
            int height;
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
                Console.WriteLine($"initiating draw of character {character} in font {font.Name}...");
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
                Console.WriteLine("...finished drawing character");

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
            public FontTable(string name, int size)
            {
                fontName = name;
                fontSize = size;
            }

            public void generateTable()
            {
                List<CharArr> tmpTable = new List<CharArr>();
                for(var i = 65; i <= 90; i++)
                {
                    string toConvert = Convert.ToChar(i).ToString();
                    CharArr toAdd = new CharArr(toConvert, fontName, fontSize);
                    tmpTable.Add(toAdd);
                }
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
            FontTable ft = new FontTable("Arial", 24);
            ft.generateTable();
            Console.WriteLine($"Generating font table of type {ft.fontName} and size {ft.fontSize}...");
            ft.printTable();

            Console.ReadLine();
        }
    }
}
