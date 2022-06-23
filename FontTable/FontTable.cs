using System;
using System.Collections.Generic;

namespace FontTable
{
    public class FontTable
    {
        #region Properties
        public List<CharArr> table = new List<CharArr>();
        public string fontName;
        public int fontSize;
        public string bitMode;
        #endregion
        public FontTable(string name, int size, string mode)
        {
            fontName = name;
            fontSize = size;
            bitMode = mode;
        }

        public void generateTable()
        {
            List<CharArr> tmpTable = new List<CharArr>();

            if (bitMode == "A")
            {
                // 7-bit mode, only produce 128 ASCII chars

                Console.WriteLine($"Generating table of type {fontName}, size {fontSize} in 7-bit mode...");
                for (var i = 32; i < 128; i++)
                {
                    string toConvert = Convert.ToChar(i).ToString();
                    CharArr toAdd = new CharArr(toConvert, fontName, fontSize);
                    tmpTable.Add(toAdd);
                }
            }
            else
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
}
