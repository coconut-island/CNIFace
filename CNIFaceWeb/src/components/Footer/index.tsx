import { DefaultFooter } from '@ant-design/pro-components';
import { useIntl } from 'umi';

const Footer: React.FC = () => {
  const intl = useIntl();
  const defaultMessage = intl.formatMessage({
    id: 'app.copyright.produced',
    defaultMessage: 'coconut-islant 组织出品',
  });

  const currentYear = new Date().getFullYear();

  return (
    <DefaultFooter
      copyright={`${currentYear} ${defaultMessage}`}
      links={[
        {
          key: 'CNIFace',
          title: 'CNIFace',
          href: 'https://github.com/coconut-island/CNIFace',
          blankTarget: true,
        },
        {
          key: 'coconut-island',
          title: <img src='/img/coconit-island-alpha-20x20.png'/>,
          href: 'https://github.com/coconut-island',
          blankTarget: true,
        },
        {
          key: 'CNIFacePlatform',
          title: 'CNIFacePlatform',
          href: 'https://github.com/coconut-island/CNIFacePlatform',
          blankTarget: true,
        },
      ]}
    />
  );
};

export default Footer;
