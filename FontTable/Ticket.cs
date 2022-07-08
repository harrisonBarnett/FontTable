using FontTable.Data;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;

namespace FontTable
{
    public class Ticket
    {
        #region Properties
        public Dictionary<string, TicketRegion> TicketRegions = new Dictionary<string, TicketRegion>();
        private string _url;
        private bool isMetric;
        private string bitMode;
        #endregion  
        public Ticket(string url)
        {
            _url = url;
            LoadTicket();
        }
        // Read a ticketdata file; set modes; generate font tables for each relevant ticket region
        private void LoadTicket()
        {
            // Sample ticket data:
            // <STX>n           <-- n = inch mode, where units are measured as 1/100 inch
            // <STX>M3417       <-- M = maximum label length, e.g. 3417 units (34 inches)
            // <STX>ICAFgfx0    <-- I = init image data; C = default memory module; A = datatype (7bit ASCII); F = format designator (7-bit Datamax-O'Neil); 
            Console.WriteLine("Reading from file...");
            try
            {
                // Parse metric/imperial mode
                string firstLine = File.ReadLines(_url).ElementAt(0);
                if (firstLine[1].ToString() == "n")
                {
                    isMetric = true;
                } else
                {
                    isMetric = false;
                }
                // Parse bitmode from datatype
                string thirdLine = File.ReadLines(_url).ElementAtOrDefault(2);
                bitMode = thirdLine[3].ToString();
            }
            catch (Exception e)
            {
                Console.WriteLine("The process failed: {0}", e.ToString());
            }
            // TODO: load ticketdata fields from database
            // for each row, newup a TicketRegion

            // TEST TEST TEST //
            TestDbData testData = new TestDbData();
            TicketRegions.Add(
                testData.NOMCMP,
                new TicketRegion(
                        testData.NOMCMP,
                        testData.NOMFUE,
                        testData.TMOFUE,
                        testData.NEGFUE == "N" ? false : true,
                        new int[4]
                        {
                            testData.TOPCNT,
                            testData.LFTCNT,
                            testData.ALTCNT,
                            testData.ANCCNT
                        },
                        bitMode,
                        isMetric
                    )
            );
            // END TEST END TEST //
        }
        // TODO: check every char in a ticketarea against the ticketarea's fonttable to produce a string
        public void Parse()
        {
            Console.WriteLine("Parsing ticket...");
        }
    }
    // Each ticketdata file has ticket regions defined by an area of [x-pos, y-pos, width, height] in FYT
    // These are associated with db objects containing other values such as font type, font size, etc.
    public class TicketRegion
    {
        #region Properties
        public string RegionName;
        public string FontFamily;
        public int FontSize;
        public bool IsBold;
        public bool IsMetric;
        public int DPI;
        public double[] RegionArea;
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
        // Convert area dimensions to pixel based on metric/imperial units provided by FYT
        private double[] ConvertToPixel(int[] area, bool isMetric)
        {
            double[] tmp = new double[4];
            if (isMetric)
            {
                for(int i = 0; i < area.Length; i++)
                {
                    tmp[i] = (area[i] / 10) * 72;
                }
            }
            else
            {
                for (int i = 0; i < area.Length; i++)
                {
                    tmp[i] = (area[i] / 100) * 72;
                }
            }

            return tmp;
        }
    }

}
