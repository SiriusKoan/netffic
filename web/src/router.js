import { createRouter, createWebHistory } from 'vue-router'
import OverviewView from './views/OverviewView.vue'
import L3View from './views/L3View.vue'
import L4View from './views/L4View.vue'
import L7View from './views/L7View.vue'

const routes = [
    { path: '/', component: OverviewView },
    { path: '/l3', component: L3View },
    { path: '/l4', component: L4View },
    { path: '/l7', component: L7View },
]

const router = createRouter({
    history: createWebHistory(),
    routes,
})

export default router