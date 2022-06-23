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
        public bool IsMetric;
        public int DPI;
        public Dictionary<string, double> RegionArea;
        public FontTable FontTable;
        #endregion
        public TicketRegion(string regionName, string fontFamily, int fontSize, bool isBold, int[] area, string bitMode, bool isMetric, int dpi)
        {
            RegionName = regionName;
            FontFamily = fontFamily;
            FontSize = fontSize;
            IsBold = isBold;
            IsMetric = isMetric;
            DPI = dpi;
            RegionArea = ConvertToPixel(area, isMetric, dpi);
            FontTable = new FontTable(fontFamily, fontSize, bitMode);
            FontTable.generateTable();
        }
        private Dictionary<string, double> ConvertToPixel(int[] area, bool isMetric, int dpi)
        {
            // TODO:
            /* convert inches/mm to px based on DPI 
             */
            double theRatio = 1;
            if(isMetric)
            {
                // do something corresponding to mm > pixel conversion at this.dpi
            } else
            {
                // do something corresponding to inch > pixel conversion at this.dpi
            }

            Dictionary<string, double> tmp = new Dictionary<string, double>();
            tmp["x"] = area[0] * theRatio;
            tmp["y"] = area[1] * theRatio;
            tmp["width"] = area[2] * theRatio;
            tmp["height"] = area[3] * theRatio;
            return tmp;
        }
    }
}
