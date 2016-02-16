#ifndef DBSVRHANDLER_HPP_INCLUDED
#define DBSVRHANDLER_HPP_INCLUDED


#include "Handler.hpp"


class DBsvrHandler: public Handler
{
public:
    DBsvrHandler (ip::tcp::socket);

    virtual void start() override;
    virtual void process_msg(int) override;

private:
    void handle_ReadInfoById();

};


void send_to_db(CMsg&);


#endif // DBSVRHANDLER_HPP_INCLUDED
