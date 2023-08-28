import { Button, Result } from 'antd';
import React from 'react';
import { history } from 'umi';

const NoFoundPage: React.FC = () => (
  <Result
    // status="404"
    title="404"
    subTitle="Sorry, the page you visited does not exist."
    icon={<img style={{height: '40vh'}} src='/icons/coconit-island-alpha-512x512.png'/>}
    extra={
      <Button type="primary" onClick={() => history.push('/')}>
        Back Home
      </Button>
    }
  />
);

export default NoFoundPage;
