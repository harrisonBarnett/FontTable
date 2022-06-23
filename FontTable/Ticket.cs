using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace FontTable
{
    public class Ticket
    {
        #region Properties
        public Dictionary<string, TicketRegion> TicketRegions;
        #endregion  
        public Ticket()
        {
            LoadTicket();
        }
        private void LoadTicket()
        {
            // TODO: load ticketdata from database
            // for each row, newup a TicketRegion
            /* new TicketRegion(
             * ticketdata.NOMFUE,
             * ticketdata.TMOFUE,
             * ticketdata.NEGFUE,
             * [
                 * ticketdata.TOPCNT,
                 * ticketdata.LFTCNT,
                 * ticketdata.ALTCNT,
                 * ticketdata.ANCCNT
             * ]
             * )*/
        }
    }

}
