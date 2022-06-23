using System.Collections.Generic;

namespace FontTable
{
    public class TicketRegion
    {
        #region Properties
        public string RegionName;
        public string FontFamily;
        public int FontSize;
        public bool IsBold;
        public Dictionary<string, int> RegionArea;
        public FontTable FontTable;
        #endregion
        public TicketRegion(string regionName, string fontFamily, int fontSize, bool isBold, int[] area, string bitMode)
        {
            RegionName = regionName;
            FontFamily = fontFamily;
            FontSize = fontSize;
            IsBold = isBold;
            RegionArea = ConvertToPixel(area);
            FontTable = new FontTable(fontFamily, fontSize, bitMode);
            FontTable.generateTable();
        }
        private Dictionary<string, int> ConvertToPixel(int[] area)
        {
            // TODO:
            /* convert "points" of location of properties in ticket (i.e. Area="x,y,width,height") to pixel
             * 72 points per inch
             * 96 pixels per inch
             * pixels = points / (72/96)?
             */
            return new Dictionary<string, int>();
        }
    }
}
