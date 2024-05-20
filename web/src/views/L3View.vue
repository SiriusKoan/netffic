<template>
    <div class="flex w-full">
        <div>
            <h2 class="text-center">IPv4</h2>
            <div class="flex w-full gap-1 items-start">
                <TwoColTable column1="Source IP address" column2="Times" :data="ipv4Stats['srcIpCounter']" />
                <TwoColTable column1="Destination IP address" column2="Times" :data="ipv4Stats['dstIpCounter']" />
            </div>
        </div>
        <div>
            <h2 class="text-center">IPv6</h2>
            <div class="flex w-full gap-1 items-start">
                <TwoColTable column1="Source IP address" column2="Times" :data="ipv6Stats['srcIpCounter']" />
                <TwoColTable column1="Destination IP address" column2="Times" :data="ipv6Stats['dstIpCounter']" />
            </div>
        </div>
    </div>
</template>

<script>
import TwoColTable from '../components/TwoColTable.vue'
export default {
    name: 'L3View',
    data() {
        return {
            ipv4Stats: {},
            ipv6Stats: {},
        }
    },
    components: {
        TwoColTable
    },
    mounted() {
        setInterval(() => {
            fetch('http://work.isk:8000/api/stats/ipv4')
                .then(response => response.json())
                .then(data => {
                    this.ipv4Stats = data
                    this.ipv4Stats['srcIpCounter'] = Object.fromEntries(
                        Object.entries(this.ipv4Stats['srcIpCounter']).sort(([, a], [, b]) => b - a)
                    )
                    this.ipv4Stats['dstIpCounter'] = Object.fromEntries(
                        Object.entries(this.ipv4Stats['dstIpCounter']).sort(([, a], [, b]) => b - a)
                    )
                })
            fetch('http://work.isk:8000/api/stats/ipv6')
                .then(response => response.json())
                .then(data => {
                    this.ipv6Stats = data
                    this.ipv6Stats['srcIpCounter'] = Object.fromEntries(
                        Object.entries(this.ipv6Stats['srcIpCounter']).sort(([, a], [, b]) => b - a)
                    )
                    this.ipv6Stats['dstIpCounter'] = Object.fromEntries(
                        Object.entries(this.ipv6Stats['dstIpCounter']).sort(([, a], [, b]) => b - a)
                    )
                })
        }, 1000)
    },
}
</script>
