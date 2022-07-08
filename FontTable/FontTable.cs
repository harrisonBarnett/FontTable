using System;
using System.Collections.Generic;
using System.Drawing;

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
    public class CharArr
    {
        #region Properties
        private string character;
        private Font font;
        private Bitmap bmp;
        private Graphics g;
        private int width;
        private int height;
        public string[] Arr; 
        #endregion
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
            g.Clear(Color.Black);
            g.TextRenderingHint = System.Drawing.Text.TextRenderingHint.ClearTypeGridFit;

            Arr = new string[height];
            generateArr();
        }
        private void generateArr()
        {
            g.DrawString(character, font, new SolidBrush(Color.White), 0, 0);

            for (int i = 0; i < height; i++)
            {
                string toAdd = "";
                for (int j = 0; j < width; j++)
                {
                    Color pixel = bmp.GetPixel(j,i);
                    toAdd += pixel.Name;
                }
                Arr[i] = toAdd;
            }
            // Flush draw memory
            g.Flush();
            g.Dispose();
        }
        public void drawChar()
        {
            for (int i = 0; i < height; i++)
            {
                Console.WriteLine(Arr[i]);
            }
        }
    }
}
