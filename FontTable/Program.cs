using System;
using System.IO;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;


namespace FontTable
{
    // TODO:
    /* convert "points" of location of properties in ticket (i.e. Area="x,y,width,height") to pixel
     * 72 points per inch
     * 96 pixels per inch
     * pixels = points / (72/96)?
     */
    // parse ticket in this area with font table at appropriate font and size

    internal class Program
    {
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
                List<CharArr> tmpTable = new List<CharArr>();

                if(bitMode == "A")
                {
                    // 7-bit mode, only produce 128 ASCII chars

                    Console.WriteLine($"Generating table of type {fontName}, size {fontSize} in 7-bit mode...");
                    for (var i = 32; i < 128; i++)
                    {
                        string toConvert = Convert.ToChar(i).ToString();
                        CharArr toAdd = new CharArr(toConvert, fontName, fontSize);
                        tmpTable.Add(toAdd);
                    }
                } else
                {
                    // 8-bit mode, produce 256 ASCII chars

                    Console.WriteLine($"Generating table of type {fontName}, size {fontSize} in 8-bit mode...");
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
            // pull printer config data
            Console.WriteLine("Reading from file...");
            string filePath = @"C:\Users\Harrison\dev\FontTable\meta\ARDFyt0000.txt.20210928 094821";
            string bitMode = "";
            try
            {
                string line = File.ReadLines(filePath).ElementAtOrDefault(2);
                bitMode = line[3].ToString();
            } catch (Exception e)
            {
                Console.WriteLine("The process failed: {0}", e.ToString());
            }

            FontTable xL = new FontTable("Arial", 24, bitMode);
            xL.generateTable();
            xL.printTable();

            Console.ReadLine();
        }
    }
}
