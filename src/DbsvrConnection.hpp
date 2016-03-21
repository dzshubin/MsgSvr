#ifndef DBSVRHANDLER_HPP_INCLUDED
#define DBSVRHANDLER_HPP_INCLUDED


#include "Connection.hpp"


class DBsvrConnection: public Connection
{
public:
    DBsvrConnection (io_service& io_);

    virtual void start() override;
    virtual void process_msg(int, string) override;

    virtual void stop_after() override;
private:
    void handle_fetch_info(string );

};


void send_to_db(CMsg&);


#endif // DBSVRHANDLER_HPP_INCLUDED
