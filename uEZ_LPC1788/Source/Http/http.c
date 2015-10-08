#include <sockets.h>
#include <tcp.h>

char mydata[1024];


static void close_conn(struct tcp_pcb *pcb){
      tcp_arg(pcb, NULL);
      tcp_sent(pcb, NULL);
      tcp_recv(pcb, NULL);
      tcp_close(pcb);
}

static err_t echo_recv(void *arg, struct tcp_pcb *pcb, struct pbuf *p, err_t err){
      int i;
      int len;
      char *pc;

      if (err == ERR_OK && p != NULL) {
            /* Inform TCP that we have taken the data. */
            tcp_recved(pcb, p->tot_len);  

            //pointer to the pay load
            pc=(char *)p->payload;  

            //size of the pay load
            len =p->tot_len; 

            //copy to our own buffer
            for (i=0; i<len; i++)mydata[i]= pc[i]; 

             //Close TCP when receiving "X"  
            if (mydata[0]=='X')close_conn(pcb); 

           //Free the packet buffer 
            pbuf_free(p);

            //check output buffer capacity 
            if (len >tcp_sndbuf(pcb)) len= tcp_sndbuf(pcb);  
            //Send out the data 
            err = tcp_write(pcb, mydata, len, 0); 
            tcp_sent(pcb, NULL); /* No need to call back */
      } else {
            pbuf_free(p);
      }

      if (err == ERR_OK && p == NULL) {
            close_conn(pcb);
      }
      return ERR_OK;
}

static err_t echo_accept(void *arg, struct tcp_pcb *pcb, err_t err){
      LWIP_UNUSED_ARG(arg);
      LWIP_UNUSED_ARG(err);
      tcp_setprio(pcb, TCP_PRIO_MIN);
      tcp_recv(pcb, echo_recv);
      tcp_err(pcb, NULL); //Don't care about error here
      tcp_poll(pcb, NULL, 4); //No polling here
      return ERR_OK;
}

void server_setup(){
      struct tcp_pcb *ptel_pcb;
      ptel_pcb = tcp_new();
      tcp_bind(ptel_pcb, IP_ADDR_ANY, 23);

      while (1){
            ptel_pcb = tcp_listen(ptel_pcb);
            tcp_accept(ptel_pcb, echo_accept);
      }
} 