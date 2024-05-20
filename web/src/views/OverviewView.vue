<template>
    <StatsContainer title="Total Stats">
        <StatViewer title="Total Packets" :value="total.totalPacketCount" :desc="last_total.totalPacketCount ? `+${total.totalPacketCount - last_total.totalPacketCount}` : ''" />
        <StatViewer title="Total Bytes" :value="bytesToSize(total.totalByteCount)" :desc="last_total.totalByteCount ? `+${bytesToSize(total.totalByteCount - last_total.totalByteCount)}` : ''" />
    </StatsContainer>
    <StatsContainer title="L3 Stats">
        <StatViewer v-for="(value, key) in l3" :key="key" :title="key" :value="value" :desc="last_l3[key] ? `+${value - last_l3[key]}` : ''" />
    </StatsContainer>
    <StatsContainer title="L4 Stats">
        <StatViewer v-for="(value, key) in l4" :key="key" :title="key" :value="value" :desc="last_l4[key] ? `+${value - last_l4[key]}` : ''" />
    </StatsContainer>
    <StatsContainer title="L7 Stats">
        <StatViewer v-for="(value, key) in l7" :key="key" :title="key" :value="value" :desc="last_l7[key] ? `+${value - last_l7[key]}` : ''" />
    </StatsContainer>
</template>

<script>
import StatsContainer from '../components/StatsContainer.vue'
import StatViewer from '../components/StatViewer.vue'
export default {
    name: 'OverviewView',
    components: {
        StatsContainer,
        StatViewer
    },
    data() {
        return {
            last_total: {},
            total: {},
            last_l3: {},
            l3: {},
            last_l4: {},
            l4: {},
            last_l7: {},
            l7: {}
        }
    },
    methods: {
        bytesToSize(bytes) {
            const sizes = ['Bytes', 'KB', 'MB', 'GB', 'TB']
            if (bytes === 0) return '0 Byte'
            const i = parseInt(Math.floor(Math.log(bytes) / Math.log(1024)))
            return Math.round(bytes / Math.pow(1024, i), 2) + ' ' + sizes[i]
        }
    },
    mounted() {
        setInterval(() => {
            fetch('http://work.isk:8000/api/stats/all')
            .then(response => response.json())
            .then(data => {
                this.last_total = this.total
                this.last_l3 = this.l3
                this.last_l4 = this.l4
                this.last_l7 = this.l7
                this.total = data.total
                this.l3 = data.l3
                this.l4 = data.l4
                this.l7 = data.l7
            })
        }, 1000)
    }
}
</script>
