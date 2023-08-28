export default [
  {
    path: '/login',
    layout: false,
    component: './Login',
  },
  {
    path: '/face_comparison',
    name: 'face_comparison',
    icon: 'OneToOne',
    component: './FaceComparison',
  },
  {
    path: '/repository',
    name: 'repository',
    icon: 'team',
    component: './Repository',
  },
  {
    path: '/repo_item',
    component: './RepoItem',
  },
  // {
  //   path: '/admin',
  //   name: 'admin',
  //   icon: 'crown',
  //   access: 'canAdmin',
  //   routes: [
  //     {
  //       path: '/admin/sub-page',
  //       name: 'sub-page',
  //       icon: 'smile',
  //       component: './Welcome',
  //     },
  //     {
  //       component: './404',
  //     },
  //   ],
  // },
  {
    path: '/',
    redirect: '/face_comparison',
  },
  {
    component: './404',
  },
];
