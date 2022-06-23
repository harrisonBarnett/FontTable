using System;
using System.Drawing;

namespace FontTable
{
    public class CharArr
    {
        #region Properties
        public string character;
        private Font font;
        private Bitmap bmp;
        private Graphics g;
        private int width;
        private int height;
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
        }
        public void drawChar()
        {
            g.DrawString(character, font, new SolidBrush(Color.White), 0, 0);

            for (int i = 0; i < height; i++)
            {
                for (int j = 0; j < width; j++)
                {
                    Color toPrint = bmp.GetPixel(j, i);
                    Console.Write(toPrint.Name);
                }
                Console.Write("\n");
            }
            // Flush draw memory
            g.Flush();
            g.Dispose();
        }
    }
}
