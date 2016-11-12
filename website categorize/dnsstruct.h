
#pragma once

struct DNSRequestHeader 
{
	unsigned short id;
	unsigned short fields;
	unsigned short qdcount;
	unsigned short ancount;
	unsigned short nscount;
	unsigned short arcount;
};
