#!/bin/bash

DB_NAME="default"

# 获取所有表的列表
TABLE_LIST=$(clickhouse-client --query="SHOW TABLES FROM $DB_NAME")

# 遍历所有表并删除它们
for TABLE_NAME in $TABLE_LIST; do
    clickhouse-client --query="DROP TABLE IF EXISTS $DB_NAME.$TABLE_NAME"
done
#!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!删除所有表，请谨慎执行！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！；