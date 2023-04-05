#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/skbuff.h>
#include <linux/in.h>
#include <linux/ip.h>
#include <linux/tcp.h>
#include <linux/icmp.h>
#include <linux/netdevice.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <linux/if_arp.h>
#include <linux/if_ether.h>
#include <linux/if_packet.h>
#include <linux/inet.h>
 
MODULE_LICENSE("GPL");
MODULE_AUTHOR("YANLIN");
 
//如果要注册一个钩子函数，必须先申明一个nf_hook_ops结构体
static struct nf_hook_ops nfho;
 
//需要过滤掉的ip——202.38.64.8   CA264008                 //需要替换成www.baidu.com的ip
static unsigned char *filter_ip = "\xb6\x3d\xc8\x06";


enum {  NF_IP_PRE_ROUTING,
        NF_IP_LOCAL_IN,
        NF_IP_FORWARD,
        NF_IP_LOCAL_OUT,
        NF_IP_POST_ROUTING,
        NF_IP_NUMHOOKS  }; 
 

//print IP address
static void dump_addr(unsigned char *iphdr)
{
    int i;
    for(i=0; i<4; i++){
        printk("%d.", *(iphdr+12+i));   
    }
    printk(" -> ");
    for(i=0; i<4; i++){
        printk("%d.", *(iphdr+16+i));   
    }
    printk("\n");
} 
/* 钩子对应的处理函数，返回值必须是以下的某一个
*NF_DROP 丢弃数据包
*NF_ACCEPT 保留该数据包
*NF_STOLEN 忘记该数据包（但不丢弃）
*NF_QUEUE 将该数据包插入用户空间
*NF_REPEAT 再次调用该hook函数
*/


unsigned int hook_func(void *priv,
                       struct sk_buff *skb,
                       const struct nf_hook_state *state){
	struct sk_buff *sb = skb;
	struct tcphdr *tcp;
    char * data;

    unsigned char *iphdr = skb_network_header(skb);
    if(iphdr){
        dump_addr(iphdr);
        if(memcmp(iphdr+16,filter_ip,4)==0){
        	return NF_DROP;
        }
    }
    return NF_ACCEPT;
}
 
 
static __init int init_HTTP(void){
        //对结构体nfho的各个属性赋值
        nfho.hook = hook_func; //处理函数的指针
        nfho.hooknum = NF_IP_LOCAL_IN;//调用钩子的时机
        nfho.pf = PF_INET;//该钩子属于哪个协议族
        nfho.priority = NF_IP_PRI_FIRST;//优先级，越小优先级越高
 
        //nf_register_net_hook(&init_net,&nfho);//将自己定义的钩子注册到内核中
       
        nf_register_hook(&nfho); //将自己定义的钩子注册到内核中
      
        return 0;
 
}
 
static __exit void cleanup_HTTP(void){
        //nf_unregister_net_hook(&init_net,&nfho);//将自己定义的钩子从内核中删
      
        nf_unregister_hook(&nfho);//将自己定义的钩子从内核中删除
}
 
module_init(init_HTTP);//entry，向内核插入模块要执行的操作
module_exit(cleanup_HTTP);//exit，从内核删除模块要执行的操作
 
