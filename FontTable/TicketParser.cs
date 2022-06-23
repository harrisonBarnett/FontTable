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
        public Ticket Ticket;
        public TicketParser(Ticket ticket)
        {
            Ticket = ticket;
        }
        public void Parse()
        {
            Console.WriteLine("parsing the ticket");
        }
    }
}
