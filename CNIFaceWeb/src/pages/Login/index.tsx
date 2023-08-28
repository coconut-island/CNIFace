import Footer from '@/components/Footer';
import { loginAPI, initAndRegisterAdminAPI } from '@/services/cniface/api';
import {
  LockOutlined,
  UserOutlined,
} from '@ant-design/icons';
import {
  LoginForm,
  ProFormText,
} from '@ant-design/pro-components';
import { Alert, Modal, message, Button, Form, Input } from 'antd';
import React, { useState } from 'react';
import { FormattedMessage, history, SelectLang, useIntl, useModel } from 'umi';
import styles from './index.less';
import { setToken } from "@/utils";

const LoginMessage: React.FC<{
  content: string;
}> = ({ content }) => (
  <Alert
    style={{
      marginBottom: 24,
    }}
    message={content}
    type="error"
    showIcon
  />
);

const Login: React.FC = () => {
  const [ userLoginState, setUserLoginState ] = useState<API.LoginResponse>({code: 0, message: "ok", result: {token: ""}});
  const { initialState, setInitialState } = useModel('@@initialState');
  const [ showInitAndRegiterAdminModal, setShowInitAndRegiterAdminModal ] = useState<boolean>(false);

  const intl = useIntl();

  const fetchUserInfo = async () => {
    const userInfo = await initialState?.fetchUserInfo?.();
    if (userInfo) {
      await setInitialState((s) => ({
        ...s,
        currentUser: userInfo,
      }));
    }
  };

  const handleSubmit = async (values: API.LoginParams) => {
    try {
      // 登录
      const response = await loginAPI({ ...values });

      if (response.code === -10) {
        message.warn(intl.formatMessage({
          id: 'pages.login.unInit',
          defaultMessage: '应用尚未初始化，请先注册admin账号！',
        }));
        setShowInitAndRegiterAdminModal(true);
        return;
      }

      if (response.code === 0) {
        const defaultLoginSuccessMessage = intl.formatMessage({
          id: 'pages.login.success',
          defaultMessage: '登录成功！',
        });
        setToken(response.result.token);
        message.success(defaultLoginSuccessMessage);
        await fetchUserInfo();
        /** 此方法会跳转到 redirect 参数所在的位置 */
        if (!history) return;
        const { query } = history.location;
        const { redirect } = query as { redirect: string };
        history.push(redirect || '/');
        return;
      }
      // 如果失败去设置用户错误信息
      setUserLoginState(response);
    } catch (error) {
      const defaultLoginFailureMessage = intl.formatMessage({
        id: 'pages.login.failure',
        defaultMessage: '登录失败，请重试！',
      });
      message.error(defaultLoginFailureMessage);
    }
  };

  const handleInitAndRegisterAdminOk = () => {
    setShowInitAndRegiterAdminModal(false);
  };

  const handleInitAndRegisterAdminCancel = () => {
    setShowInitAndRegiterAdminModal(false);
  };

  const onInitAndRegisterAdminFinish = async (values: any) => {
    const { password } = values;
    const response = await initAndRegisterAdminAPI({password});
    if (response.code === 0) {
      message.success(
        intl.formatMessage({
          id: 'pages.login.initAndRegisterAdminSubmit.success',
          defaultMessage: '初始化以及注册成功',
        })
      );
      setShowInitAndRegiterAdminModal(false);
    } else {
      message.error(
        intl.formatMessage({
          id: 'pages.login.initAndRegisterAdminSubmit.fail',
          defaultMessage: '初始化以及注册失败',
        })
      );
    }
  };

  const onInitAndRegisterAdminFinishFailed = (errorInfo: any) => {
    console.error('Failed:', errorInfo);
  };

  const { code } = userLoginState;

  return (
    <div className={styles.container}>
      <div className={styles.lang} data-lang>
        {SelectLang && <SelectLang />}
      </div>
      <div className={styles.content}>
        <LoginForm
          logo={<img alt="logo" src="/icons/coconit-island-alpha-128x128.png" />}
          title="CNIFace"
          subTitle={intl.formatMessage({ id: 'pages.layouts.userLayout.title' })}
          initialValues={{
            autoLogin: true,
          }}
          actions={[
          ]}
          onFinish={async (values) => {
            await handleSubmit(values as API.LoginParams);
          }}
        >

          {code !== 0 && (
            <LoginMessage
              content={intl.formatMessage({
                id: 'pages.login.accountLogin.errorMessage',
                defaultMessage: '账户或密码错误',
              })}
            />
          )}
          <ProFormText
            name="username"
            fieldProps={{
              size: 'large',
              prefix: <UserOutlined className={styles.prefixIcon} />,
            }}
            placeholder={intl.formatMessage({
              id: 'pages.login.username.placeholder',
              defaultMessage: '用户名',
            })}
            rules={[
              {
                required: true,
                message: (
                  <FormattedMessage
                    id="pages.login.username.required"
                    defaultMessage="请输入用户名!"
                  />
                ),
              },
            ]}
          />
          <ProFormText.Password
            name="password"
            fieldProps={{
              size: 'large',
              prefix: <LockOutlined className={styles.prefixIcon} />,
            }}
            placeholder={intl.formatMessage({
              id: 'pages.login.password.placeholder',
              defaultMessage: '密码',
            })}
            rules={[
              {
                required: true,
                message: (
                  <FormattedMessage
                    id="pages.login.password.required"
                    defaultMessage="请输入密码！"
                  />
                ),
              },
            ]}
          />
        </LoginForm>
      </div>
      <Modal
        title={
          intl.formatMessage({
            id: 'pages.login.adminRegisterModelTitle',
            defaultMessage: 'admin账号注册',
          })
        }
        visible={showInitAndRegiterAdminModal}
        onOk={handleInitAndRegisterAdminOk}
        onCancel={handleInitAndRegisterAdminCancel}
        closable={false}
        keyboard={false}
        maskClosable={false}
        footer={null}
      >
        <Form
          name="basic"
          onFinish={onInitAndRegisterAdminFinish}
          onFinishFailed={onInitAndRegisterAdminFinishFailed}
          autoComplete="off"
        >
          <Form.Item
            wrapperCol={{ offset: 4, span: 16 }}
            name="password"
            rules={[
              {
                required: true,
                message: (
                  <FormattedMessage
                    id="pages.login.password.required"
                    defaultMessage="请输入密码！"
                  />
                ),
              },
            ]}
          >
            <Input.Password
              prefix={
                <LockOutlined className={styles.prefixIcon} />
              }
              placeholder={intl.formatMessage({
                id: 'pages.login.password.placeholder',
                defaultMessage: '密码',
              })}
            />
          </Form.Item>

          <Form.Item wrapperCol={{ offset: 10 }}>
            <Button type={'ghost'} htmlType="submit">
              {
                intl.formatMessage({
                  id: 'pages.login.adminRegisterModelOkButton',
                  defaultMessage: '提交',
                })
              }
            </Button>
          </Form.Item>
        </Form>
      </Modal>
      <Footer />
    </div>
  );
};

export default Login;
