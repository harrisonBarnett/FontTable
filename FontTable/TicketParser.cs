using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

/* TODO //
 * ticketparser accepts a new Ticket
 * Parse() diffs region by region against the appropriate font table
*/

namespace FontTable
{
    public class TicketParser
    {
        public Ticket _ticket;
        public TicketParser(string url)
        {
            _ticket = new Ticket(url);
        }

        public void Parse()
        {
            Console.WriteLine("parsing the ticket");
        }
    }
}
