package cn.abellee.cniface.platform.domain.entity.mysql;

import org.hibernate.dialect.MySQL5InnoDBDialect;

/**
 * @author abel
 * @date 2022/10/7 10:16 PM
 */

public class DefaultMySQL57InnoDBDialect extends MySQL5InnoDBDialect {
    @Override
    public String getTableTypeString() {
        return " ENGINE=InnoDB DEFAULT CHARSET=utf8";
    }
}
