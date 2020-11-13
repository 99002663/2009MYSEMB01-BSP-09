#include<linux/module.h>
#include<linux/netdevice.h>
#include<linux/platform_device.h>
#include<linux/init.h>
#include<linux/errno.h>
#include<linux/ethtool.h>
#include<linux/etherdevice.h>
#include<linux/kernel.h>
#include<linux/slab.h>
#include<linux/skbuff.h>
#include<linux/of.h>

struct struc_ether {
	int bar;
	int foo;
	struct net_device *dummy_net;		//creation of a structure pointer of type net_device 
};

//Open function written to start the net device -- defined in netdevice.h
static int dummy_eth_open(struct net_device *dev)		
{
	printk("dummy_eth_open is called\n");
	netif_start_queue(dev);
	return 0;
}

//Releasing function written to disconstruc_ethernect the net device -- defined in netdevice.h
static int dummy_eth_release(struct net_device *dev)		
{
	pr_info("dummy_eth_release is called\n");
	netif_stop_queue(dev); // Stop the layers calling the device xmit routine. It is used for flow control when the resources which have to be txed are not available.
	return 0;
}

//Xmit function written to start the net device -- defined in netdevice.h
static int dummy_eth_xmit(struct sk_buff *skb, struct net_device *ndev) 
{
	pr_info("dummy_xmit is called...\n");
	ndev->stats.tx_bytes += skb->len;	// the skb structure pointer is used to assign the no of bytes using the len 
	ndev->stats.tx_packets++;	// after eevery trasmit pointer is moved to the next packet

	skb_tx_timestamp(skb);	
	dev_kfree_skb(skb);	// freeing the space occupied by the skb buffer
	return NETDEV_TX_OK;	// will send the acknowledgement 
}

//Initializing function written for the initial setup of the net device -- defined in netdevice.h
static int dummy_eth_init(struct net_device *dev)	
{
	pr_info("dummy net device intialized\n"); // will be visible in the kernel by using the lsmod command
	return 0;
};

/**Structure creation where all the file operations are linked
 defined in netdevice.h
**/
static const struct net_device_ops my_netdev_ops = {		
	.ndo_init = dummy_eth_init,
	.ndo_open = dummy_eth_open,
	.ndo_stop = dummy_eth_release,
	.ndo_start_xmit = dummy_eth_xmit,
	.ndo_validate_addr = eth_validate_addr,
};

//this structure is used for matching the device
static const struct of_device_id dummy_eth_dt_ids[] = {
	{ .compatible = "pack dummy-eth",},
};

static int dummy_eth_probe(struct platform_device *pdev)
{
	int ret;
	struct struc_ether *priv;
	struct net_device *dummy_net;

	priv = devm_kzalloc(&pdev->dev, sizeof(*priv), GFP_KERNEL);//this function pointer is returning an inline function which is stored into a structure pointer
	if(!priv)
	return 0;

	dummy_net = alloc_etherdev(sizeof(struct struc_ether)); // the macro alloc_etherdev is used to give the size of the structureand align the dummy net driver with same size
	dummy_net->if_port = IF_PORT_10BASET;		//unsigned char
	dummy_net->netdev_ops = &my_netdev_ops;		// structure of type net_device

	ret = register_netdev(dummy_net);	// register_netdev(struct net_device *dev)-- returns the status of the registration of network device
	if(ret)
	{
		pr_info("dummy network device-%d ",ret);
		return ret;
	}
	priv->dummy_net = dummy_net; // the dummy device created is assigned to the dummy device of the priv structure.
	platform_set_drvdata(pdev, priv); // this function returns a inline function where the value stored in priv structure is assigned to the pdev structure(platform_device.h)
	return 0;
	}

/**Platform devices are inherently not discoverable, i.e. the hardware is not self discoverable 
Present in the platform_device.h file
**/
static int dummy_eth_remove(struct platform_device *pdev)	
{
	struct struc_ether *priv;
	priv = platform_get_drvdata(pdev); // A pointer which takes the pdev structure as the input and returns a inline funcn which is linked to the struct priv
	pr_info("removing the net device");
	unregister_netdev(priv->dummy_net); // Unregistering the dummy net device to end the process
	free_netdev(priv->dummy_net);	// freeing up the space of the net device
	return 0;
}	

// Platform structure is linked with all the operations done using it which is defined in the platform_device.h
static struct platform_driver mypdrv = {
	.probe = dummy_eth_probe,
	.remove = dummy_eth_remove,
	.driver = {
	.name = "dummy_eth",
	.of_match_table = of_match_ptr(dummy_eth_dt_ids),
	.owner = THIS_MODULE,
	},
};

module_platform_driver(mypdrv);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("TEAM NINE");
MODULE_DESCRIPTION("NET DRIVER");
