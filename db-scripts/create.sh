#!/bin/sh

# This defines parameters
# parameters.sql 

# This creates db and user
# create-db-user.sql

# This creates db schema (tables)
# schema.sql

# This adds triggers to tables
# triggers.sql


# This does all
sudo -u postgres psql postgres -f parameters.sql -f create-db-user.sql  -f schema.sql -f triggers.sql -f grant.sql