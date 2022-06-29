using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace FontTable.Data
{
    // NB: This is taken from the first row of sample table of ticket data
    public class TestDbData
    {
        public string NOMCMP = "CODPRO";    // field
        public string NOMFUE = "Arial";     // font name
        public int TMOFUE = 39;             // font size
        public string NEGFUE = "N";         // bold (to be converted to bool)
        public int TOPCNT = 3675;           // y position of region
        public int LFTCNT = 2400;           // x position of region
        public int ALTCNT = 1170;           // height of region
        public int ANCCNT = 6225;           // width of region
    }
}
