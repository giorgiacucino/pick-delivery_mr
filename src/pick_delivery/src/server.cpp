#include <ros/ros.h>
#include <stdlib.h>
#include <list>
#include "users.h"
#include "aula.h"
#include "robot.h"
#include "pick_delivery/login.h"
#include "/home/me/labiagi_2020_21/workspaces/srrg2_labiagi/devel/include/srrg2_core_ros/PlannerStatusMessage.h"
#include "pick_delivery/invio.h"
#include "pick_delivery/notifica.h"
#include "pick_delivery/s_to_c.h"
#include "pick_delivery/c_to_s.h"
#include <fstream>
#include <geometry_msgs/PoseStamped.h>
using namespace std;

list<user>					userlist;
list<aula>					aulelist;
robot					    rob[2];
pick_delivery::s_to_c		n;
geometry_msgs::PoseStamped	goal;
ros::Publisher				pub_robot[2];
ros::Subscriber				sub_robot[2];
ros::ServiceServer			servLog;
ros::ServiceServer			servSend;
ros::Publisher				pub_client;
ros::Subscriber				sub_client;
int                         num_robot;

bool	handle_client(pick_delivery::login::Request &req, pick_delivery::login::Response &res)
{
	if (req.type_service == 0)
	{
		for (auto& u : userlist)
		{
			if (u.hash == req.name && u.can_logout == 1)
			{
				userlist.remove(u);
				res.serv_resp = "Bye bye!";
				return (1);
			}
			else if (u.hash == req.name)
			{
				res.serv_resp = "Il robot sta venendo da te, aspettalo";
				return (0);
			}
		}
	}
	if (req.type_service == 1)
	{
		int	count;

		count = 0;
		for (auto& u : userlist)
		{
			if (u.name==req.name)
			{
				count++;
			}
		}
		user us = user(req.name, count);
		userlist.push_back(us);
		res.serv_resp = us.hash;
	}
	if (req.type_service == 2)
	{
		int	valid;

		valid = 0;
		for (auto& u : userlist)
		{
			if (u.hash == req.name)
			{
				if (u.can_logout == 0)
					res.serv_resp = "Sta arrivando un pacco per te, non puoi cambiare aula";
				else
					res.serv_resp = "Aula non valida, riprova";
				for (auto& a : aulelist)
				{
					if (a.name == req.aula)
					{
						valid = 1;
						u.x = a.x;
						u.y = a.y;
						break ;
					}
				}
			}
		}
		if (valid == 1)
			res.serv_resp = req.aula;
		else
			return (0);
	}
	return (true);
}

void	log_all_users(aula *a, int log)
{
	for (auto& u : userlist)
	{
		if (u.x == (*a).x && u.y == (*a).y)
			u.can_logout = log;
	}
}

void	get_infos(robot r[2], int i, string sen, string recv, string auladst)
{
	if (recv == "0")
		r[i].broadcast = 1;
	for (auto& u : userlist)
	{
		if (u.hash == sen)
		{
			r[i].sender = &u;
			r[i].sender->can_logout = 0;
		}
		if (r[i].broadcast == 0 && u.hash == recv)
		{
			r[i].receiver = &u;
			r[i].receiver->can_logout = 0;
		}
	}
	for (auto& a : aulelist)
	{
		if (a.name == auladst)
			r[i].auladest = &a;
	}
	if (r[i].broadcast == 1)
		log_all_users(r[i].auladest, 0);
}

void	set_goal(int i)
{
	int	coordx;
	int coordy;
	cout << "[INFO] Stato del robot_" << i << ": " << rob[i].status << endl;
	if (rob[i].status == 0)
		return ;
	if (rob[i].status == 1)
	{
		coordx = rob[i].sender->x;
		coordy = rob[i].sender->y;
	}
	if (rob[i].status == 2)
	{
		coordx = rob[i].auladest->x;
		coordy = rob[i].auladest->y;
	}
	cout << "[INFO] Il robot deve andare alla coordinata " << coordx << " " << coordy << endl;
	goal.header.stamp = ros::Time::now();
	goal.header.frame_id = "map";
	goal.pose.position.x = coordx;
	goal.pose.position.y = coordy;
	cout << "[INFO] Goal settato a " << coordx << " " << coordy << endl;
	pub_robot[i].publish(goal);
	ros::spinOnce();
}

void	callback_Server(const pick_delivery::c_to_s::ConstPtr& msg)
{
	cout << "[INFO] Il client ha risposto con robot_" << msg->idrob << " e sto cambiando lo stato in " << msg->resp << endl;
	int i = msg->idrob;
	rob[i].status = msg->resp;
	if (rob[i].status == 2)
	{
		rob[i].sender->can_logout = 1;
		cout << "[INFO] Il sender può anche uscire" << endl;
		set_goal(i);
	}
	else if (rob[i].status == 0)
	{
		log_all_users(rob[i].auladest, 1);
		cout << "[INFO] I receiver possono anche uscire" << endl;
		rob[i].free = 1;
	}
}

void 	callClient(int i)
{
	if (rob[i].status == 0)
		return ;
	else if (rob[i].status == 1)
	{
		rob[i].sender->called = 1;
		n.user = rob[i].sender->hash;
		n.auladest = "";
		n.msg = "E' arrivato il robot e ha preso in consegna il pacco";
		n.pd = 1;
	}
	else if (rob[i].status == 2)
	{
		rob[i].auladest->called = 1;
		if (rob[i].broadcast == 1)
			n.user = "0";
		else
			n.user = rob[i].receiver->hash;
		n.auladest = rob[i].auladest->name;
		n.msg = "C'è un pacco per te, ritiralo";
		n.pd = 2;
	}
	n.idrob = i;
	cout << "[INFO] Aspetto risposta dal client..." << endl;
	pub_client.publish(n);
	ros::spinOnce();
}

bool	handle_invio(pick_delivery::invio::Request &req, pick_delivery::invio::Response &res)
{
	res.serv_resp = 0;
	int onefree = 0;
	int i;
	for (i = 0; i < num_robot; i++)
	{
		if (rob[i].free == 1)
		{
			onefree = 1;
			rob[i].free = 0;
			break ;
		}
	}
	if (onefree == 0)
		res.serv_resp = 2;
	else
	{
		int	room;
		int person;

		room = 0;
		person = 0;
		for (auto& a : aulelist)
		{
			if (a.name == req.aula)
			{
				room = 1;
				if (req.receiver == "0")
				{
					person = 1;
					break ;
				}
				for (auto& u : userlist)
				{
					if (u.hash == req.receiver && a.x == u.x && a.y == u.y)
					{
						person = 1;
						break ;
					}
				}
			}
		}
		if (room == 0 && person == 1)
		{
			rob[i].free = 1;
			res.serv_resp = 3;
		}
		else if (person == 0)
		{
			rob[i].free = 1;
			res.serv_resp = 4;
		}
		else if (room == 1 && person == 1)
		{
			res.serv_resp = 1;
			get_infos(rob, i, req.sender, req.receiver, req.aula);
			cout << "[INFO] Richiesta di pick&delivery accettata" << endl;
			cout << "sender: " << rob[i].sender->hash << endl;
			cout << "receiver: " << req.receiver << endl;
			cout << "aula: " << rob[i].auladest->name << endl;
			rob[i].status = 1;
			cout << "[INFO] Settando un nuovo goal..." << endl;
			set_goal(i);
			cout << "[INFO] Nuovo goal settato" << endl;
			cout << "[INFO] Informando il receiver..." << endl;
			if (rob[i].broadcast == 1)
				n.user = "0";
			else
				n.user = rob[i].receiver->hash;
			n.auladest = rob[i].auladest->name;
			n.msg = "Sta arrivando un pacco per te, attendi!";
			n.pd = 0;
			pub_client.publish(n);
			cout << "[INFO] Receiver informato" << endl;
		}
	}
	return (true);
}

void	check_robot(const srrg2_core_ros::PlannerStatusMessage::ConstPtr& info, int num)
{
	rob[num].distance = info->distance_to_global_goal;
	//cout << "[INFO] La distanza del robot_" << num << " dal goal è: " << rob[num].distance << endl;
	if (rob[num].distance < 0.7)
	{
		int client_called = 0;
		if (rob[num].status == 1)
			client_called = rob[num].sender->called;
		else if (rob[num].status == 2)
			client_called = rob[num].auladest->called;
		if (rob[num].prevdist >= 0.7 && rob[num].distance != rob[num].prevdist && rob[num].distance != 0 && client_called == 0)
		{
			cout << "[INFO] Il robot_" << num << " è arrivato a destinazione" << endl;
			cout << "[INFO] Sto chiamando il client..." << endl;
			callClient(num);
		}
	}
	rob[num].prevdist = info->distance_to_global_goal;
}

int	main(int argc, char **argv)
{
	ros::init(argc, argv, "server");
	
	ros::NodeHandle		node;

    if (argc == 1)
    {
        cout << "errore: inserisci il numero di robot" << endl;
        return (0);
    }
    
	num_robot = atoi(argv[1]);
    string mvbase = "/move_base_simple/goal";
    string plstat = "/planner_status";
	
	servLog = node.advertiseService("/newclient", handle_client);
	servSend = node.advertiseService("/invio", handle_invio);

	pub_client = node.advertise<pick_delivery::s_to_c>("/stoc", 1000);
	sub_client = node.subscribe("/ctos", 1000, callback_Server);
    
	for (int i = 0; i < num_robot; i++)
    {
        rob[i] = robot();
        string robot = "/robot_" + to_string(i);
	    pub_robot[i] = node.advertise<geometry_msgs::PoseStamped>(robot + mvbase, 1000);
	    sub_robot[i] = node.subscribe<srrg2_core_ros::PlannerStatusMessage>(robot + plstat, 1000, boost::bind(check_robot, _1, i) );
    }

	aulelist = get_aule();
	
	ros::spin();

	return (0);
}
