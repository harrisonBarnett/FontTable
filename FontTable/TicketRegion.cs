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
        public TicketRegion(string regionName, string fontFamily, int fontSize, bool isBold, int[] area, string bitMode, bool isMetric)
        {
            RegionName = regionName;
            FontFamily = fontFamily;
            FontSize = fontSize;
            IsBold = isBold;
            IsMetric = isMetric;
            RegionArea = ConvertToPixel(area, isMetric);
            FontTable = new FontTable(fontFamily, fontSize, bitMode);
            FontTable.generateTable();
        }
        private Dictionary<string, double> ConvertToPixel(int[] area, bool isMetric)
        {
            Dictionary<string, double> tmp = new Dictionary<string, double>();
            tmp["x"] = 0;
            tmp["y"] = 0;
            tmp["width"] = 0;
            tmp["height"] = 0;
            // TODO:
            /*
             * if isMetric, area[n] / 10
             * else, area[n] / 100 * 72
             */
            if(isMetric)
            {
                int i = 0;
                foreach (var key in tmp.Keys)
                {
                    tmp[key] = (area[i] / 10) * 72;
                    i++;
                }
            } else
            {
                int i = 0;
                foreach(var key in tmp.Keys)
                {
                    tmp[key] = (area[i] / 100) * 72;
                    i++;
                }
            }

            return tmp;
        }
    }
}
