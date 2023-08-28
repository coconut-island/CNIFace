import { Settings as LayoutSettings } from '@ant-design/pro-components';

const Settings: LayoutSettings & {
  pwa?: boolean;
  logo?: string;
} = {
  navTheme: 'light',
  // 拂晓蓝
  primaryColor: '#1890ff',
  layout: 'top',
  contentWidth: 'Fluid',
  fixedHeader: false,
  fixSiderbar: true,
  colorWeak: false,
  title: 'CNIFace',
  pwa: false,
  logo: 'icons/coconit-island-alpha-192x192.png',
  iconfontUrl: '',
  headerHeight: 48,
  splitMenus: false,
};

export default Settings;
